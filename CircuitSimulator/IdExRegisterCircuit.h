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
	InputPin m_rt_in;
	InputPin m_rd_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;
	
	// WB
	OutputPin m_regWrite_out;		// 1bit : [146, 146]
	OutputPin m_memToReg_out;		// 1bit : [145, 145]
	// MEM
	OutputPin m_branch_out;			// 1bit : [144, 144]
	OutputPin m_memRead_out;		// 1bit : [143, 143]
	OutputPin m_memWrite_out;		// 1bit : [142, 142]
	// EX
	OutputPin m_regDest_out;		// 1bit : [141, 141]
	OutputPin m_aluOp_out;			// 2bit : [139, 140]
	OutputPin m_aluSrc_out;			// 1bit : [138, 138]
	/* ================ */
	OutputPin m_pc_out;				// 32bit : [106. 137]
	OutputPin m_read1_out;			// 32bit : [74, 105]
	OutputPin m_read2_out;			// 32bit : [42, 73]
	OutputPin m_signExtended_out;	// 32bit : [10, 41]
	OutputPin m_rt_out;				// 5bit : [5, 9]
	OutputPin m_rd_out;				// 5bit : [0, 4]

	bool m_outBuf1[147];
	bool m_outBuf2[147];

	uint32_t m_data[14];
	char m_strBuf[256];
};

#endif
