#include "stdafx.h"
#include "ForwardCircuit.h"
#include "Convert.h"

ForwardCircuit::ForwardCircuit()
	: Circuit("Forward Unit", ECircuitType::ForwardUnit, 4, 2, m_outBuf1, m_outBuf2, 4, 0.1f)
	, m_rt(*this, "rt", 5)
	, m_rs(*this, "rs", 5)
	, m_exMemAluReg(*this, "EX/MEM Reg", 5)
	, m_exMemRegwrite(*this, "EX/MEM RegW", 1)
	, m_memWbAluReg(*this, "MEM/WB Reg", 5)
	, m_memWbRegWrite(*this, "MEM/WB RegW", 1)
	, m_forwardA(*this, "A", 0, 2)
	, m_forwardB(*this, "B", 2, 2)
{}

ForwardCircuit::ForwardCircuit(float x, float y)
	: ForwardCircuit()
{
	SetPos(x, y);
}
