#ifndef IF_ID_REGISTER_CIRCUIT_H
#define IF_ID_REGISTER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class IfIdRegisterCircuit final : public Circuit
{
public:
	IfIdRegisterCircuit();
	IfIdRegisterCircuit(float x, float y);

	void Render() override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_pc_in;		// 32bit
	InputPin m_instruction_in; // 32bit

	// TODO: Add This
	InputPin m_write;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	OutputPin m_pc_out;		// 32bit : [63, 94]
	OutputPin m_op_out;		//  6bit : [57, 62]
	OutputPin m_rs_out;		//  5bit : [52, 56]
	OutputPin m_rt_out;		//  5bit : [47, 51]
	OutputPin m_rd_out;		//  5bit : [42, 46]
	OutputPin m_low16_out;	// 16bit : [26 ,41]
	OutputPin m_addr_out;	// 26bit : [ 0, 25]

	bool m_outBuf1[95];
	bool m_outBuf2[95];

	uint32_t m_data[7];
};

#endif
