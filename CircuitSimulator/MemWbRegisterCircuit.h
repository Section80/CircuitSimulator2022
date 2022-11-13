#ifndef MEM_WB_REGISTER_CIRCUIT_H
#define MEM_RB_REGISTER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class MemWbRegisterCircuit final : public Circuit
{
public:
	MemWbRegisterCircuit();
	MemWbRegisterCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	// WB
	InputPin m_regWrite_in;
	InputPin m_memToReg_in;
	/* ================ */
	InputPin m_readData_in;
	InputPin m_aluResult_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	// WB
	OutputPin m_regWrite_out;		// 1bit : [65, 65]
	OutputPin m_memToReg_out;		// 1bit : [64, 64]
	/* ================ */
	OutputPin m_readData_out;		// 1bit : [32, 63]
	OutputPin m_aluResult_out;		// 1bit : [0, 31]

	bool m_outBuf1[66];
	bool m_outBuf2[66];

	uint32_t m_data[4];
};

#endif
