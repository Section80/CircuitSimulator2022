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

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_op;				// 5-bit op field from instruction

	// WB
	OutputPin m_regWrite;	// 1-bit : [5, 5]
	OutputPin m_memToReg;	// 1-bit : [6, 6]
	// MEM
	OutputPin m_branch;		// 1-bit : [2, 2]
	OutputPin m_memRead;	// 1-bit : [4, 4]
	OutputPin m_memWrite;	// 1-bit : [3, 3]
	// EX
	OutputPin m_regDest;	// 1-bit : [8, 8]
	OutputPin m_aluOp;		// 2-bit : [0, 1]
	OutputPin m_aluSrc;		// 1-bit : [7, 7]

	bool m_outBuf1[9];
	bool m_outBuf2[9];
};

#endif
