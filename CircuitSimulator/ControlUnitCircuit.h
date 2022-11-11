#ifndef CONTROL_UNIT_CIRCUIT
#define CONTROL_UNIT_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

// 교재 307p 참고

class ControlUnitCircuit final : public Circuit
{
public:
	ControlUnitCircuit();
	ControlUnitCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_op;				// 5-bit op field from instruction

	// WB
	OutputPin m_regWrite;		// whether to write register file or not
	OutputPin m_memToReg;		// from alu result(0) or data memory(1)
	// MEM
	OutputPin m_branch;			// pc + 4(0) or bta(1)
	OutputPin m_memRead;
	OutputPin m_memWrite;
	// EX
	OutputPin m_regDest;		// rt(0) or rd(1) for write register
	OutputPin m_aluOp;
	OutputPin m_aluSrc;			// use read data2(0) or sign-extended(1)

	bool m_outBuf1[9];
	bool m_outBuf2[9];
};

#endif
