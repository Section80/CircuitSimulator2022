#include "stdafx.h"
#include "Instruction.h"

#define MASK_RS		0b00000011111000000000000000000000
#define MASK_RT		0b00000000000111110000000000000000
#define MASK_RD		0b00000000000000001111100000000000
#define MASK_LOW16	0b00000000000000001111111111111111

int SetRS(int instruction, int rs)
{
	instruction &= ~MASK_RS;
	
	rs = rs << 21;
	rs &= MASK_RS;

	instruction |= rs;
	return instruction;
}

int SetRT(int instruction, int rt)
{
	instruction &= ~MASK_RT;

	rt = rt << 16;
	rt &= MASK_RT;

	instruction |= rt;
	return instruction;
}

int SetRD(int instruction, int rd)
{
	instruction &= ~MASK_RD;

	rd = rd << 11;
	rd &= MASK_RD;

	instruction |= rd;
	return instruction;
}

int SetLow16(int instruction, int low16)
{
	instruction &= ~MASK_LOW16;
	
	low16 &= MASK_LOW16;

	instruction |= low16;
	return instruction;

	return 0;
}

Instruction::Instruction(int val)
	: m_val(val)
{}

Instruction::Instruction()
	: m_val(0)
{}

Instruction& Instruction::SetRS(int rs)
{
	m_val = ::SetRS(m_val, rs);

	return *this;
}

Instruction& Instruction::SetRT(int rt)
{
	m_val = ::SetRT(m_val, rt);

	return *this;
}

Instruction& Instruction::SetRD(int rd)
{
	m_val = ::SetRD(m_val, rd);

	return *this;
}

Instruction& Instruction::SetLow16(int low16)
{
	m_val = ::SetRS(m_val, low16);

	return *this;
}
