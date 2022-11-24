#include "stdafx.h"
#include "Instruction.h"

#define MASK_OP			0b11111100000000000000000000000000
#define MASK_RS			0b00000011111000000000000000000000
#define MASK_RT			0b00000000000111110000000000000000
#define MASK_RD			0b00000000000000001111100000000000
#define MASK_FUNCT		0b00000000000000000000000000111111
#define MASK_LOW16		0b00000000000000001111111111111111
#define MASK_ADDRESS	0b00000011111111111111111111111111

int SetOpcode(int instruction, int opcode)
{
	instruction &= ~MASK_OP;

	opcode = opcode << 26;
	opcode &= MASK_OP;

	instruction |= opcode;
	return instruction;
}

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

int SetFunct(int instruction, int funct)
{
	instruction &= ~MASK_FUNCT;
	funct &= MASK_FUNCT;

	instruction |= funct;
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

int SetAddress(int instruction, int address)
{
	instruction &= ~MASK_ADDRESS;
	address &= MASK_ADDRESS;

	instruction |= address;
	return instruction;
}

Instruction::Instruction(int val)
	: m_val(val)
{}

Instruction::Instruction()
	: m_val(0)
{}

Instruction& Instruction::SetOpcode(int opcode)
{
	m_val = ::SetOpcode(m_val, opcode);
	return *this;
}

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

Instruction& Instruction::SetFunct(int funct)
{
	m_val = ::SetFunct(m_val, funct);

	return *this;
}

Instruction& Instruction::SetLow16(int low16)
{
	m_val = ::SetLow16(m_val, low16);

	return *this;
}

Instruction& Instruction::SetAddress(int address)
{
	m_val = ::SetAddress(m_val, address);

	return *this;
}
