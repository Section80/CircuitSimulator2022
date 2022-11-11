#ifndef EX_MEM_REGISTER_CIRCUIT_H
#define EX_MEM_REGISTER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class ExMemRegisterCircuit final : public Circuit
{
public:
	ExMemRegisterCircuit();
	ExMemRegisterCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	// MEM
	InputPin m_branch_in;			// pc + 4(0) or bta(1)
	InputPin m_memRead_in;
	InputPin m_memWrite_in;
	// EX
	InputPin m_regDest_in;		// rt(0) or rd(1) for write register
	InputPin m_aluOp_in;
	InputPin m_aluSrc_in;			// use read data2(0) or sign-extended(1)
	/* ================ */
	InputPin m_bta_in;
	InputPin m_zero_in;
	InputPin m_aluResult_in;
	InputPin m_read2_in;
	InputPin m_writeReg_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	// MEM
	OutputPin m_branch_out;			// 1bit : [108, 108]
	OutputPin m_memRead_out;		// 1bit : [107, 107]
	OutputPin m_memWrite_out;		// 1bit : [106, 106]
	// EX
	OutputPin m_regDest_out;		// 1bit : [105, 105]
	OutputPin m_aluOp_out;			// 2bit : [103, 104]
	OutputPin m_aluSrc_out;			// 1bit : [102, 102]
	/* ================ */
	OutputPin m_bta_out;			// 32bit : [70, 101]
	OutputPin m_zero_out;			// 1bit : [69, 69]
	OutputPin m_aluResult_out;		// 32bit : [37, 68]
	OutputPin m_read2_out;			// 32bit : [5, 36]
	OutputPin m_writeReg_out;		// 5bit : [0, 4]

	bool m_outBuf1[109];
	bool m_outBuf2[109];

	uint32_t m_data[11];
};

#endif