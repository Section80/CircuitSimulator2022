#include "stdafx.h"
#include "SpawnCircuit.h"

#include "Circuit.h"
#include "AndCircuit.h"
#include "SwitchCircuit.h"
#include "BitBulbCircuit.h"
#include "NotCircuit.h"
#include "BufferCircuit.h"
#include "OrCircuit.h"
#include "Int32OutCircuit.h"
#include "DisplayInt32Circuit.h"
#include "RegIdxOutCircuit.h"
#include "RegisterCircuit.h"
#include "DataMemoryCircuit.h"
#include "ClockCircuit.h"
#include "ProgramCounterCircuit.h"
#include "Add32Circuit.h"
#include "InstructionMemory.h"
#include "SignExtensionCircuit.h"
#include "ShiftLeftCircuit.h"
#include "MuxCircuit.h"
#include "AluCircuit.h"
#include "AluControlCircuit.h"
#include "ControlUnitCircuit.h"
#include "IfIdRegisterCircuit.h"
#include "IdExRegisterCircuit.h"
#include "ExMemRegisterCircuit.h"
#include "MemWbRegisterCircuit.h"
#include "FunctCircuit.h"
#include "ForwardCircuit.h"

void connect(Circuit* pC1, int outIdx, Circuit* pC2, int inIdx);

void SpawnTestRegisterFile(float x, float y, std::vector<Circuit*>* pCircuits)
{
	// input
	RegIdxOutCircuit* reg1 = new RegIdxOutCircuit(x - 300, y - 100);
	pCircuits->push_back(reg1);

	RegIdxOutCircuit* reg2 = new RegIdxOutCircuit(x - 300, y - 50);
	pCircuits->push_back(reg2);
	RegIdxOutCircuit* reg3 = new RegIdxOutCircuit(x - 300, y);
	pCircuits->push_back(reg3);
	Int32OutCircuit* wData = new Int32OutCircuit(x - 300, y + 70);
	pCircuits->push_back(wData);
	SwitchCircuit* s1 = new SwitchCircuit(x - 200, y + 130);
	pCircuits->push_back(s1);
	SwitchCircuit* s2 = new SwitchCircuit(x - 300, y + 190);
	pCircuits->push_back(s2);

	RegisterCircuit* r = new RegisterCircuit(x, y);
	pCircuits->push_back(r);
	reg1->GetOutputPin(0)->ConnectNew(r->GetInputPin(0));
	reg2->GetOutputPin(0)->ConnectNew(r->GetInputPin(1));
	reg3->GetOutputPin(0)->ConnectNew(r->GetInputPin(2));
	wData->GetOutputPin(0)->ConnectNew(r->GetInputPin(3));
	s1->GetOutputPin(0)->ConnectNew(r->GetInputPin(4));
	s2->GetOutputPin(0)->ConnectNew(r->GetInputPin(5));

	// output
	DisplayInt32Circuit* i1 = new DisplayInt32Circuit(x + 300, y - 50);
	pCircuits->push_back(i1);
	DisplayInt32Circuit* i2 = new DisplayInt32Circuit(x + 300, y + 50);
	pCircuits->push_back(i2);
	r->GetOutputPin(0)->ConnectNew(i1->GetInputPin(0));
	r->GetOutputPin(1)->ConnectNew(i2->GetInputPin(0));

	// etc
	SwitchCircuit* s3 = new SwitchCircuit(x -400, y + 190);
	pCircuits->push_back(s3);
	DataMemoryCircuit* d = new DataMemoryCircuit(x, y - 150 );
	pCircuits->push_back(d);
}

#define AddXY(name, clss, _x, _y) ;clss* name = new clss(x + (_x), y + (_y)); (pCircuits)->push_back(name);


void SpawnSimple1(float x, float y, std::vector<Circuit*>* pCircuits)
{
	AddXY(clock, ClockCircuit, -200.0f, 150.0f);
	AddXY(pc, ProgramCounterCircuit, -350.0f, 0);
	// connect(clock, 0, pc, 2);

	AddXY(iVal4, Int32OutCircuit, -600.0f, -150.0f);
	AddXY(pcAdd, Add32Circuit, -350.0f, -100.0f);
	// connect(iVal4, 0, pcAdd, 0);
	// connect(pc, 0, pcAdd, 1);
	// connect(pcAdd, 0, pc, 0);

	AddXY(pcWrite, SwitchCircuit, - 500.0f, 0);
	// connect(pcWrite, 0, pc, 1);

	AddXY(instMem, InstructionMemoryCircuit, -150.0f, 0);
	// connect(pc, 0, instMem, 0);

	AddXY(ifid, IfIdRegisterCircuit, -50, 0);
	// connect(pcAdd, 0, ifid, 0);
	// connect(instMem, 0, ifid, 1);
	// connect(instMem, 1, ifid, 2);
	// connect(instMem, 2, ifid, 3);
	// connect(instMem, 3, ifid, 4);
	// connect(instMem, 4, ifid, 5);
	// connect(instMem, 5, ifid, 6);
	// connect(clock, 0, ifid, 7);

	AddXY(regFile, RegisterCircuit, 50.0f, 0);
	// connect(instMem, 1, regFile, 0);
	// connect(instMem, 2, regFile, 1);
	// connect(instMem, 3, regFile, 2);
	// connect(clock, 0, regFile, 5);
	
	AddXY(exe, Add32Circuit, 250, 0);
	// connect(regFile, 0, exe, 0);
	// connect(regFile, 1, exe, 1);

	AddXY(dataMem, DataMemoryCircuit, 450.0f, 0);
	// connect(exe, 0, dataMem, 0);
	// connect(clock, 0, dataMem, 4);

	AddXY(signEx, SignExtensionCircuit, 0, 250.0f);
	AddXY(shift, ShiftLeftCircuit, 100.0f, 250.0f);
	AddXY(mux, Mux21Circuit, 200.0f, 250.0f);
	
	AddXY(i1, Int32OutCircuit, 100.0f, 250.0f);
	AddXY(i2, Int32OutCircuit, 200.0f, 250.0f);
	AddXY(alu, AluCircuit, 300.0f, 250.0f);
	AddXY(display_i, DisplayInt32Circuit, 300.0f, 250.0f);
	AddXY(aluOp, AluOperationCircuit, 400.0f, 250.0f);
	AddXY(buf, BufferCircuit, 500.0f, 250.0f);
	AddXY(aluCtrl, AluControlCircuit, 600.0f, 250.0f);
	AddXY(ctrlUnit, ControlUnitCircuit, 700.0f, 250.0f);
	
	AddXY(idex, IdExRegisterCircuit, -600.0f, 350.0f);
	AddXY(exMem, ExMemRegisterCircuit, -500.0f, 350.0f);
	AddXY(memWb, MemWbRegisterCircuit, -400.0f, 350.0f);
	// AddXY(buf, BufferCircuit, x + 250, y + 100.0f);
	// connect(dataMem, 0, buf, 0);
	// connect(buf, 0, regFile, 3);
}

Circuit* SpawnCircuit(ECircuitType type, float x, float y)
{
	switch (type)
	{
	case ECircuitType::None:
		printf("[Info]SpawnCircuit Invalid id: %d", (int)type);
		return nullptr;
	case ECircuitType::Add32:
		return new Add32Circuit(x, y);
	case ECircuitType::ALU:
		return new AluCircuit(x, y);
	case ECircuitType::ALUOperation:
		return new AluOperationCircuit(x, y);
	case ECircuitType::ALUControl:
		return new AluControlCircuit(x, y);
	case ECircuitType::ControlUnit:
		return new ControlUnitCircuit(x, y);
	case ECircuitType::InstructionMemory:
		return new InstructionMemoryCircuit(x, y);
	case ECircuitType::ShiftLeft:
		return new ShiftLeftCircuit(x, y);
	case ECircuitType::SignExtention:
		return new SignExtensionCircuit(x, y);
	case ECircuitType::BitBulb:
		return new BitBulbCircuit(x, y);
	case ECircuitType::DisplayInt32:
		return new DisplayInt32Circuit(x, y);
	case ECircuitType::Int32Out:
		return new Int32OutCircuit(x, y);
	case ECircuitType::RegIdxOut:
		return new RegIdxOutCircuit(x, y);
	case ECircuitType::Switch:
		return new SwitchCircuit(x, y);
	case ECircuitType::And:
		return new AndCircuit(x, y);
	case ECircuitType::Nand:
		return new NandCircuit(x, y);
	case ECircuitType::Buffer:
		return new BufferCircuit(x, y);
	case ECircuitType::Mux21:
		return new Mux21Circuit(x, y);
	case ECircuitType::Mux31:
		return new Mux31Circuit(x, y);
	case ECircuitType::Not:
		return new NotCircuit(x, y);
	case ECircuitType::Or:
		return new OrCircuit(x, y);
	case ECircuitType::Nor:
		return new NorCircuit(x, y);
	case ECircuitType::ExMem:
		return new ExMemRegisterCircuit(x, y);
	case ECircuitType::IdEx:
		return new IdExRegisterCircuit(x, y);
	case ECircuitType::IfId:
		return new IfIdRegisterCircuit(x, y);
	case ECircuitType::MemWb:
		return new MemWbRegisterCircuit(x, y);
	case ECircuitType::Clock:
		return new ClockCircuit(x, y);
	case ECircuitType::DataMemory:
		return new DataMemoryCircuit(x, y);
	case ECircuitType::ProgramCounter:
		return new ProgramCounterCircuit(x, y);
	case ECircuitType::Register:
		return new RegisterCircuit(x, y);
	case ECircuitType::Funct:
		return new FunctCircuit(x, y);
	case ECircuitType::ClockBuffer:
		return new ClockBufferCircuit(x, y);
	case ECircuitType::ForwardUnit:
		return new ForwardCircuit(x, y);
	default:
		printf("[Info]SpawnCircuit Invalid id: %d", (int)type);
		break;
	}

	return nullptr;
}

void connect(Circuit* pC1, int outIdx, Circuit* pC2, int inIdx)
{
	pC1->GetOutputPin(outIdx)->ConnectNew(pC2->GetInputPin(inIdx));
}