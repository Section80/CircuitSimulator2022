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

#define AddXY(name, clss, x, y) ;clss* name = new clss(x, y); (pCircuits)->push_back(name);



void SpawnSimple1(float x, float y, std::vector<Circuit*>* pCircuits)
{
	AddXY(clock, ClockCircuit, x - 200.0f, y + 150.0f);
	AddXY(pc, ProgramCounterCircuit, x - 350.0f, y);
	connect(clock, 0, pc, 2);

	AddXY(iVal4, Int32OutCircuit, x - 600.0f, y - 150.0f);
	AddXY(pcAdd, Add32Circuit, x - 350.0f, y - 100.0f);
	connect(iVal4, 0, pcAdd, 0);
	connect(pc, 0, pcAdd, 1);
	connect(pcAdd, 0, pc, 0);

	AddXY(pcWrite, SwitchCircuit, x - 500.0f, y);
	connect(pcWrite, 0, pc, 1);

	AddXY(instMem, InstructionMemoryCircuit, x - 150.0f, y);
	connect(pc, 0, instMem, 0);

	AddXY(regFile, RegisterCircuit, x + 50.0f, y);
	connect(instMem, 0, regFile, 0);
	connect(instMem, 1, regFile, 1);
	connect(instMem, 2, regFile, 2);
	connect(clock, 0, regFile, 5);
	
	AddXY(exe, Add32Circuit, x + 250, y);
	connect(regFile, 0, exe, 0);
	connect(regFile, 1, exe, 1);

	AddXY(dataMem, DataMemoryCircuit, x + 450.0f, y);
	connect(exe, 0, dataMem, 0);
	connect(clock, 0, dataMem, 4);

	AddXY(signEx, SignExtensionCircuit, x, y + 250.0f);
	AddXY(shift, ShiftLeftCircuit, x + 100.0f, y + 250.0f);
	AddXY(mux, Mux21Circuit, x + 200.0f, y + 250.0f);
	
	AddXY(i1, Int32OutCircuit, x - 100.0f, y + 250.0f);
	AddXY(i2, Int32OutCircuit, x - 200.0f, y + 250.0f);
	AddXY(alu, AluCircuit, x + 300.0f, y + 250.0f);
	AddXY(display_i, DisplayInt32Circuit, x - 300.0f, y + 250.0f);
	AddXY(aluOp, AluOperationCircuit, x - 400.0f, y + 250.0f);

	AddXY(buf, BufferCircuit, x - 500.0f, y + 250.0f);

	// AddXY(buf, BufferCircuit, x + 250, y + 100.0f);
	// connect(dataMem, 0, buf, 0);
	// connect(buf, 0, regFile, 3);
}

void connect(Circuit* pC1, int outIdx, Circuit* pC2, int inIdx)
{
	pC1->GetOutputPin(outIdx)->ConnectNew(pC2->GetInputPin(inIdx));
}