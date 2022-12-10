#include "stdafx.h"
#include <assert.h>
#include <imgui_node_editor.h>
#include "DataMemory.h"

namespace ImNode = ax::NodeEditor;

DataMemory::DataMemory()
	: m_lastChanged(-1)
{}

int DataMemory::SetAscii(int address, std::string& ascii)
{
	for (char c : ascii)
	{
		SetByte(address++, c);
	}

	return int(address + ascii.size());
}

int DataMemory::SetAsciiz(int address, std::string& ascii)
{
	for (char c : ascii)
	{
		SetByte(address++, c);
	}
	SetByte(address++, 0);

	return int(address + ascii.size() + 1);
}

int DataMemory::SetSpace(int address)
{
	m_map[address] = 0;
	return address + 1;
}

int DataMemory::SetByte(int address, char val)
{
	m_map[address] = val;

	return address + 1;
}

int DataMemory::SetHalf(int address, short val)
{
	// algin
	int remainder = address % 2;
	if (remainder != 0)
	{
		address += (2 - remainder);
	}

	char* p = (char*)&val;
	SetByte(address + 0, p[0]);
	SetByte(address + 1, p[1]);

	return address + 2;
}

int DataMemory::SetWord(int address, int val)
{
	// word algin
	int remainder = address % 4;
	if (remainder != 0)
	{
		address += (4 - remainder);
	}

	char* p = (char*)&val;
	SetByte(address + 0, p[0]);
	SetByte(address + 1, p[1]);
	SetByte(address + 2, p[2]);
	SetByte(address + 3, p[3]);

	return address + 4;
}

int DataMemory::SetFloat(int address, float val)
{
	// word algin
	int remainder = address % 4;
	if (remainder != 0)
	{
		address += (4 - remainder);
	}

	char* p = (char*)&val;
	SetByte(address + 0, p[0]);
	SetByte(address + 1, p[1]);
	SetByte(address + 2, p[2]);
	SetByte(address + 3, p[3]);

	return address + 4;
}

int DataMemory::SetDouble(int address, double val)
{
	// algin
	int remainder = address % 8;
	if (remainder != 0)
	{
		address += (8 - remainder);
	}

	char* p = (char*)&val;
	SetByte(address + 0, p[0]);
	SetByte(address + 1, p[1]);
	SetByte(address + 2, p[2]);
	SetByte(address + 3, p[3]);
	SetByte(address + 4, p[4]);
	SetByte(address + 5, p[5]);
	SetByte(address + 6, p[6]);
	SetByte(address + 7, p[7]);

	return address + 8;
}

void DataMemory::SetLastChangd(int address)
{
	m_lastChanged = address;
}

void DataMemory::Clear()
{
	m_map.clear();
}

int DataMemory::ReadWord(int address)
{
	assert(address % 4 == 0);

	char c0 = 0;
	if (m_map.count(address + 0))
	{
		c0 = m_map[address + 0];
	}

	char c1 = 0;
	if (m_map.count(address + 1))
	{
		c1 = m_map[address + 1];
	}

	char c2 = 0;
	if (m_map.count(address + 2))
	{
		c2 = m_map[address + 2];
	}

	char c3 = 0;
	if (m_map.count(address + 3))
	{
		c3 = m_map[address + 3];
	}

	int ret = 0;
	
	char* pRet = (char*)&ret;

	pRet[0] = c0;
	pRet[1] = c1;
	pRet[2] = c2;
	pRet[3] = c3;

	return ret;
}

void DataMemory::RenderInspector()
{
	int lastWordAddress = -1;

	for (const auto& pair : m_map)
	{
		int key = pair.first;
		char val = pair.second;

		int share = key / 4;
		int wordAddress = share * 4;
		if (wordAddress > lastWordAddress)
		{
			int word = ReadWord(wordAddress);
			lastWordAddress = wordAddress;

			if (m_lastChanged == wordAddress)
			{
				// yellow
				if (word == 0)
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[%0#10x] 0x00000000", key);
				}
				else
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[%0#10x] %0#10x", key, word);
				}
			}
			else
			{
				if (word == 0)
				{
					ImGui::Text("[%0#10x] 0x00000000", key);
				}
				else
				{
					ImGui::Text("[%0#10x] %0#10x", key, word);
				}
			}
		}
	}
}
