#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "SpawnCircuit.h"
#include "File.h"
#include "MuxCircuit.h"
#include "Int32OutCircuit.h"
#include "InstructionMemory.h"


void SaveCircuitsToFile(const char* fileName, std::vector<Circuit*>& circuits)
{
	using namespace std;

	// NodeId는 프로그램이 실행된 후 발급되는 것임으로 저장할 때는
	// 별도의 아이디를 사용해야 한다. 
	int lastCircuitId = -1;
	unordered_map<int, int> idMap;	// key : NodeId, val: CircuitId

	ofstream f(fileName, ofstream::out | ofstream::trunc);

	if (!f.is_open())
	{
		printf("Save %s FAIL! \n", fileName);
		return;
	}

	for (auto c : circuits)
	{
		// id
		idMap[c->GetId()] = ++lastCircuitId;
		f << lastCircuitId << " ";

		// type
		f << (int)c->GetType() << " ";
		
		// pos
		ImVec2 pos = c->GetPos();
		f << pos.x << " ";
		f << pos.y;

		if (c->GetType() == ECircuitType::Mux21)
		{
			Mux21Circuit* m = (Mux21Circuit*)c;
			f << " " << m->GetWireLineCount();
		} 
		else if (c->GetType() == ECircuitType::Mux31)
		{
			Mux31Circuit* m = (Mux31Circuit*)c;
			f << " " << m->GetWireLineCount();
		}
		else if (c->GetType() == ECircuitType::Buffer)
		{
			BufferCircuit* b = (BufferCircuit*)c;
			if (b->IsReversed())
			{
				f << " " << 1 << " ";
			}
			else
			{
				f << " " << 0 << " ";
			}

			f << b->GetWireLineCount();
		}
		else if (c->GetType() == ECircuitType::Switch)
		{
			SwitchCircuit* s = (SwitchCircuit*)c;
			if (s->GetPressed())
			{
				f << " " << 1;
			}
			else 
			{
				f << " " << 0;
			}
		}
		else if (c->GetType() == ECircuitType::Int32Out)
		{
			Int32OutCircuit* i32 = (Int32OutCircuit*)c;
			f << " " << i32->GetValue();
		}
		else if (c->GetType() == ECircuitType::ClockBuffer)
		{
			ClockBufferCircuit* b = (ClockBufferCircuit*)c;
			if (b->IsReversed())
			{
				f << " " << 1;
			}
			else
			{
				f << " " << 0;
			}
		}
		else if (c->GetType() == ECircuitType::InstructionMemory)
		{
			InstructionMemoryCircuit* i = (InstructionMemoryCircuit*)c;
			f << " " << i->GetPath();
		}

		// new line
		f << std::endl;
	}

	// end of circuit. now start pin
	f << "-1" << std::endl;

	for (auto c : circuits)
	{
		int outCnt = c->GetOutputPinCount();

		for (int i = 0; i < outCnt; i++)
		{
			OutputPin* out = c->GetOutputPin(i);
			for (int j = 0; j < MAX_WIRE_IN_OUTPUTPIN; j++)
			{
				Wire& w =out->GetWire(j);
				InputPin* in = w.GetTo();
				if (in != nullptr)
				{
					Circuit& target = in->GetOwner();
					// c의 i번째 출력 핀이 target의 in에 연결돼어 있다.
					// in은 몇번째 입력핀일까? 
					int inCnt = target.GetInputPinCount();
					for (int k = 0; k < inCnt; k++)
					{
						// k번째 입력핀이다. 
						if (in == target.GetInputPin(k))
						{
							// c의 i번째 출력 핀이 target의 k번쨰 입력핀에 연결돼어 있다.
							f << idMap[c->GetId()] << " ";
							f << i << " ";
							f << idMap[target.GetId()] << " ";
							f << k << " ";
							f << endl;
							break;
						}
					}
				}
			}
		}
	}

	// end of file
	f << "-1" << std::endl;

	f.close();
}

void LoadCircuitsFromFile(const char* fileName, std::vector<Circuit*>* pCircuits)
{
	using namespace std;

	unordered_map<int, Circuit*> map;

	ifstream f(fileName, ifstream::in);

	if (!f.is_open())
	{
		printf("Load %s FAIL! \n", fileName);
		return;
	}

	// 처음에는 Pin이 아닌 Circuits을 읽으면서 시작한다. 
	bool isReadingPin = false;

	while (!isReadingPin)
	{
		// read circuitId
		int id = 0;
		f >> id;

		if (id == -1)
		{
			isReadingPin = true;
			break;
		}

		// read type
		ECircuitType type = ECircuitType::None;
		f >> (int&)type;
		// read Position
		float x = 0.0f;
		float y = 0.0f;
		f >> x;
		f >> y;

		Circuit* c = SpawnCircuit(type, x, y);
		map[id] = c;
		pCircuits->push_back(c);

		if (type == ECircuitType::Mux21)
		{
			Mux21Circuit* m = (Mux21Circuit*)c;
			int wireLineCount = 0;
			f >> wireLineCount;
			m->SetWireLineCount(wireLineCount);
		}
		else if (type == ECircuitType::Mux31)
		{
			Mux31Circuit* m = (Mux31Circuit*)c;
			int wireLineCount = 0;
			f >> wireLineCount;
			m->SetWireLineCount(wireLineCount);
		}
		else if (type == ECircuitType::Buffer)
		{
			BufferCircuit* b = (BufferCircuit*)c;
			int isReversed = 0;
			f >> isReversed;
			if (isReversed == 1)
			{
				b->SetIsReversed(true);
			}
			else 
			{
				b->SetIsReversed(false);
			}

			int wireLineCount = 0;
			f >> wireLineCount;
			b->SetWireLineCount(wireLineCount);
		}
		else if (type == ECircuitType::Switch)
		{
			SwitchCircuit* s = (SwitchCircuit*)c;
			int isPressed = 0;
			f >> isPressed;
			if (isPressed)
			{
				s->SetPressed(true);
			}
			else
			{
				s->SetPressed(false);
			}
		} 
		else if (type == ECircuitType::Int32Out)
		{
			Int32OutCircuit* i32 = (Int32OutCircuit*)c;
			int value = 0;
			f >> value;
			i32->SetValue(value);
		}
		else if (type == ECircuitType::ClockBuffer)
		{
			ClockBufferCircuit* cb = (ClockBufferCircuit*)c;
			int isReversed = 0;
			f >> isReversed;
			if (isReversed == 1)
			{
				cb->SetIsReversed(true);
			}
			else
			{
				cb->SetIsReversed(false);
			}
		}
		else if (type == ECircuitType::InstructionMemory)
		{
			InstructionMemoryCircuit* in = (InstructionMemoryCircuit*)c;
			std::string path;
			f >> path;
			if (path.compare("NULL") != 0)
			{
				in->LoadInstruction(path);
			}
		}
	}

	int cnt = 0;
	// now start reading pins
	while (true)
	{
		
		int from = -1;
		f >> from;
		Circuit* cFrom = map[from];

		if (from == -1)
		{
			break;
		}

		cnt++;
		printf("[info]load pin count: %d \n", cnt);

		int outIndex = -1;
		f >> outIndex;

		int to = -1;
		f >> to;
		Circuit* cTo = map[to];

		int inIndex = -1;
		f >> inIndex;

		OutputPin* outPin = cFrom->GetOutputPin(outIndex);
		InputPin* inPin = cTo->GetInputPin(inIndex);
		outPin->ConnectNew(inPin);
	}

	f.close();

}
