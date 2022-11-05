#include <assert.h>
#include "Pin.h"
#include "Wire.h"

std::vector<Wire*> Wire::s_pWires;

Wire::Wire()
	: Identifiable()
	, m_from(nullptr)
	, m_to(nullptr)
{
	m_linkId = ImNode::LinkId(GetId());
	s_pWires.push_back(this);
};

Wire::~Wire()
{
	s_pWires.erase(
		std::remove(s_pWires.begin(), s_pWires.end(), this),
		s_pWires.end()
	);
}

void Wire::Render()
{
	assert(m_from != nullptr);

	if (m_to != nullptr)
	{
		ImNode::Link(m_linkId, m_from->GetPinId(), m_to->GetPinId());
	}
}

Wire* Wire::GetWireByLinkId(ImNode::LinkId linkId)
{
	auto it = std::find_if(
		s_pWires.begin(),
		s_pWires.end(),
		[linkId](Wire* pWire)
		{
			return pWire->GetLinkId() == linkId;
		}
	);

	if (it == s_pWires.end()) {
		return nullptr;
	}

	return (*it);
}