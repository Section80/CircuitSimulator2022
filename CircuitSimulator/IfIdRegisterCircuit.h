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
	InputPin m_pc_in;		// 32bit : [32, 63]
	InputPin m_instruction_in; // 32bit : [0, 31]

	// TODO: Add This
	// InputPin m_write

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	OutputPin m_pc_out;		// 32bit : [32, 63]
	OutputPin m_op_out;		// 6bit : [26, 31]
	OutputPin m_rs_out;		// 5bit : [21, 25]
	OutputPin m_rt_out;		// 5bit : [16, 20]
	OutputPin m_rd_out;		// 5bit : [11, 15]
	OutputPin m_low16_out;	// 16bit : [0,15]

	bool m_outBuf1[64];
	bool m_outBuf2[64];

	uint32_t m_data[7];
};

#endif
