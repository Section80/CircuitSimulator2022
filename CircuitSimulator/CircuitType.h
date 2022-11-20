#ifndef CIRCUIT_TYPE_H
#define CIRCUIT_TYPE_H

enum class ECircuitType: int
{
	None = 0,
	Add32 = 1,
	ALU = 2,
	ALUOperation = 3, 
	ALUControl = 4,
	ControlUnit = 5,
	InstructionMemory = 6,
	ShiftLeft = 7,
	SignExtention = 8,
	BitBulb = 9,
	DisplayInt32 = 10,
	Int32Out = 11,
	RegIdxOut = 12,
	Switch = 13,
	And = 14,
	Nand = 15, 
	Buffer = 16,
	Mux21 = 17,
	Mux31 = 18,
	Not = 19,
	Or = 20,
	Nor = 21, 
	ExMem = 22,
	IdEx = 23,
	IfId = 24,
	MemWb = 25,
	Clock = 26,
	DataMemory = 27,
	ProgramCounter = 28,
	Register = 29,
	Funct = 30,
	ClockBuffer = 31, 
	ForwardUnit = 32, 
	LoadStoreUnit = 33, 
	HarzardDetection = 34, 
};

#endif

