#ifndef INSTRUCTION_H
#define INSTRUCTION_H

int SetRS(int instruction, int rs);
int SetRT(int instruction, int rt);
int SetRD(int instruction, int rd);
int SetLow16(int instruction, int rt);

class Instruction
{
public :
	Instruction(int val);
	Instruction();

	Instruction& SetRS(int rs);
	Instruction& SetRT(int rt);
	Instruction& SetRD(int rd);
	Instruction& SetLow16(int low16);

	inline int Get() { return m_val; }

private:
	int m_val;
};

#endif