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

void SpawnTestRegisterFile(float x, float y, std::vector<Circuit*> pCircuits)
{
	// input
	RegIdxOutCircuit* reg1 = new RegIdxOutCircuit(x - 300, y - 100);
	pCircuits.push_back(reg1);
	RegIdxOutCircuit* reg2 = new RegIdxOutCircuit(x - 300, y - 50);
	pCircuits.push_back(reg2);
	RegIdxOutCircuit* reg3 = new RegIdxOutCircuit(x - 300, y);
	pCircuits.push_back(reg3);
	Int32OutCircuit* wData = new Int32OutCircuit(x - 300, y + 70);
	pCircuits.push_back(wData);
	SwitchCircuit* s1 = new SwitchCircuit(x - 200, y + 130);
	pCircuits.push_back(s1);
	SwitchCircuit* s2 = new SwitchCircuit(x - 300, y + 190);
	pCircuits.push_back(s2);

	RegisterCircuit* r = new RegisterCircuit(x, y);
	pCircuits.push_back(r);
	reg1->GetOutputPin(0)->ConnectNew(r->GetInputPin(0));
	reg2->GetOutputPin(0)->ConnectNew(r->GetInputPin(1));
	reg3->GetOutputPin(0)->ConnectNew(r->GetInputPin(2));
	wData->GetOutputPin(0)->ConnectNew(r->GetInputPin(3));
	s1->GetOutputPin(0)->ConnectNew(r->GetInputPin(4));
	s2->GetOutputPin(0)->ConnectNew(r->GetInputPin(5));

	// output
	DisplayInt32Circuit* i1 = new DisplayInt32Circuit(x + 300, y - 50);
	pCircuits.push_back(i1);
	DisplayInt32Circuit* i2 = new DisplayInt32Circuit(x + 300, y + 50);
	pCircuits.push_back(i2);
	r->GetOutputPin(0)->ConnectNew(i1->GetInputPin(0));
	r->GetOutputPin(1)->ConnectNew(i2->GetInputPin(0));


	// etc
	SwitchCircuit* s3 = new SwitchCircuit(x -400, y + 190);
	pCircuits.push_back(s3);
	DataMemoryCircuit* d = new DataMemoryCircuit(x, y - 150 );
	pCircuits.push_back(d);
}
