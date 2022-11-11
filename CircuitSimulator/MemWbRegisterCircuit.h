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
	InputPin m_readData_in;
	InputPin m_aluResult_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	OutputPin m_readData_out;
	OutputPin m_aluResult_out;

	bool m_outBuf1[64];
	bool m_outBuf2[64];

	uint32_t m_data[2];
};

#endif
