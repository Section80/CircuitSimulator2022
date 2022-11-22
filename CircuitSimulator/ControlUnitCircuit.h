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
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_instruction;

	// WB
	OutputPin m_regWrite;	// 1-bit : [9, 9]
	OutputPin m_memToReg;	// 1-bit : [8, 8]
	// MEM
	OutputPin m_branch;		// 1-bit : [7, 7]
	OutputPin m_jump;		// 1-bit : [6, 6]
	OutputPin m_memRead;	// 1-bit : [5, 5]
	OutputPin m_memWrite;	// 1-bit : [4, 4]
	// EX
	OutputPin m_regDest;	// 1-bit : [3, 3]
	OutputPin m_aluOp;		// 2-bit : [1, 2]
	OutputPin m_aluSrc;		// 1-bit : [0, 0]

	bool m_outBuf1[10];
	bool m_outBuf2[10];
};

#endif
