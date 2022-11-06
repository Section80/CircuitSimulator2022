#include "stdafx.h"
#include "Pin.h"
#include "Circuit.h"

std::vector<Pin*> Pin::s_pPins;

Pin::Pin(Circuit& owner, const char* name, ImNode::PinKind pinkind, int wireLineCount)
	: Identifiable()
	, m_owner(owner)
	, m_name(name)
	, m_pinKind(pinkind)
	, m_wireLineCount(wireLineCount)
{
	m_pinId = ImNode::PinId(GetId());
	s_pPins.push_back(this);

}

Pin::~Pin() 
{
	s_pPins.erase(
		std::remove(s_pPins.begin(), s_pPins.end(), this), 
		s_pPins.end()
	);
}

Pin* Pin::GetPinById(ImNode::PinId pinId) {
	auto it = std::find_if(
		s_pPins.begin(),
		s_pPins.end(),
		[pinId](Pin* pPin)
		{
			return pPin->GetPinId() == pinId;

		}
	);

	if (it == s_pPins.end()) {
		return nullptr;
	}

	return (*it);
}

void Pin::InitVector()
{
	s_pPins.reserve(1000);
}

InputPin::InputPin(Circuit& owner, const char* name, int wireLineCount)
	: Pin(owner, name, ImNode::PinKind::Input, wireLineCount)
	, m_from(nullptr)
{}

InputPin::~InputPin()
{}

void InputPin::Render()
{
	ImNode::BeginPin(GetPinId(), ImNode::PinKind::Input);
	char buffer[64] = { 0 };
	sprintf_s(buffer, "> %s[%d]", GetName(), GetWireLineCount());
	ImGui::Text(buffer);
	ImNode::EndPin();
}

bool InputPin::ReadAt(int wireLinIndex)
{
	assert(m_from != nullptr);
	return m_from->ReadAt(wireLinIndex);
}

OutputPin::OutputPin(Circuit& owner, const char* name, int dataOffset, int wireLineCount)
	: Pin(owner, name, ImNode::PinKind::Output, wireLineCount)
	, m_dataOffset(dataOffset)
{
	for (int i = 0; i < MAX_WIRE_IN_OUTPUTPIN; i++)
	{
		m_wires[i].m_from = this;
	}
}

OutputPin::~OutputPin()
{}

void OutputPin::Render()
{
	ImNode::BeginPin(GetPinId(), ImNode::PinKind::Output);
	char buffer[64] = { 0 };
	sprintf_s(buffer, "%s[%d] >", GetName(), GetWireLineCount());
	ImGui::Text(buffer);
	ImNode::EndPin();
}

void OutputPin::RenderWire()
{
	for (int i = 0; i < MAX_WIRE_IN_OUTPUTPIN; i++)
	{
		m_wires[i].Render();
	}
}

Wire& OutputPin::GetWire(int index)
{
	assert(index < MAX_WIRE_IN_OUTPUTPIN);

	return m_wires[index];
}

bool OutputPin::ReadAt(int index) { 
	assert(index < GetWireLineCount());
	return GetOwner().GetOutputData(m_dataOffset + index); 
}

bool OutputPin::CanConnectNew()
{
	for (int i = 0; i < MAX_WIRE_IN_OUTPUTPIN; i++)
	{
		if (m_wires[i].m_to == nullptr)
		{
			return true;
		}
	}

	return false;
}

bool OutputPin::ConnectNew(InputPin* pInputPin)
{
	assert(pInputPin != nullptr);
	assert(GetWireLineCount() == pInputPin->GetWireLineCount());

	for (int i = 0; i < MAX_WIRE_IN_OUTPUTPIN; i++)
	{
		if (m_wires[i].m_to == nullptr)
		{
			if (pInputPin->m_from != nullptr)
			{
				bool bRet = pInputPin->m_from->Disconnect(pInputPin);
				assert(bRet);
			}

			m_wires[i].m_to = pInputPin;
			pInputPin->m_from = this;
			
			if (pInputPin->GetOwner().IsResolvable())
			{
				pInputPin->GetOwner().onInputChanged();
			}

			return true;
		}
	}

	return false;
}

bool OutputPin::Disconnect(InputPin* pInputPin)
{
	assert(pInputPin != nullptr);

	for (int i = 0; i < MAX_WIRE_IN_OUTPUTPIN; i++)
	{
		if (m_wires[i].m_to == pInputPin)
		{
			m_wires[i].m_to = nullptr;
			pInputPin->m_from = nullptr;

			return true;
		}
	}
	return false;
}