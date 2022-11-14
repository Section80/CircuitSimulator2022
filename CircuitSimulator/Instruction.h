#ifndef INSTRUCTION_H
#define INSTRUCTION_H

int SetOpcode(int instruction, int opcode);
int SetRS(int instruction, int rs);
int SetRT(int instruction, int rt);
int SetRD(int instruction, int rd);
int SetFunct(int instruction, int funct);
int SetLow16(int instruction, int rt);
int SetAddress(int instruction, int address);


class Instruction
{
public :
	Instruction(int val);
	Instruction();

	Instruction& SetOpcode(int opcode);
	Instruction& SetRS(int rs);
	Instruction& SetRT(int rt);
	Instruction& SetRD(int rd);
	Instruction& SetFunct(int funct);
	Instruction& SetLow16(int low16);
	Instruction& SetAddress(int address);

	inline int Get() { return m_val; }

private:
	int m_val;
};

#endif