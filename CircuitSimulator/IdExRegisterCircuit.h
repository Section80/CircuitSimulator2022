#ifndef EX_MEM_REGISTER_CIRCUIT_H
#define ID_EX_REGISTER_CIRCUIT_H


#include "Circuit.h"
#include "Pin.h"

class IdExRegisterCircuit final : public Circuit
{
public:
	IdExRegisterCircuit();
	IdExRegisterCircuit(float x, float y);

	void render() override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	// WB
	InputPin m_regWrite_in;		// whether to write register file or not
	InputPin m_memToReg_in;		// from alu result(0) or data memory(1)
	// MEM
	InputPin m_branch_in;			// pc + 4(0) or bta(1)
	InputPin m_memRead_in;
	InputPin m_memWrite_in;
	// EX
	InputPin m_regDest_in;		// rt(0) or rd(1) for write register
	InputPin m_aluOp_in;
	InputPin m_aluSrc_in;			// use read data2(0) or sign-extended(1)
	/* ================ */
	InputPin m_pc_in;
	InputPin m_read1_in;
	InputPin m_read2_in;
	InputPin m_signExtended_in;
	InputPin m_rs_in;
	InputPin m_rt_in;
	InputPin m_rd_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;
	
	// WB
	OutputPin m_regWrite_out;		// 1bit : [151, 151]
	OutputPin m_memToReg_out;		// 1bit : [150, 150]
	// MEM
	OutputPin m_branch_out;			// 1bit : [149, 149]
	OutputPin m_memRead_out;		// 1bit : [148, 148]
	OutputPin m_memWrite_out;		// 1bit : [147, 147]
	// EX
	OutputPin m_regDest_out;		// 1bit : [146, 146]
	OutputPin m_aluOp_out;			// 2bit : [144, 145]
	OutputPin m_aluSrc_out;			// 1bit : [143, 143]
	/* ================ */
	OutputPin m_pc_out;				// 32bit : [111, 142]
	OutputPin m_read1_out;			// 32bit : [79, 110]
	OutputPin m_read2_out;			// 32bit : [47, 78]
	OutputPin m_signExtended_out;	// 32bit : [15, 46]
	OutputPin m_rs_out;				// 5bit : [10, 14]
	OutputPin m_rt_out;				// 5bit : [5, 9]
	OutputPin m_rd_out;				// 5bit : [0, 4]

	bool m_outBuf1[152];
	bool m_outBuf2[152];

	uint32_t m_data[14];
	char m_strBuf[256];
};

#endif
