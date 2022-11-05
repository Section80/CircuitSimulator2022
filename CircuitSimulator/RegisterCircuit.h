#ifndef REGISTER_CIRCUIT_H
#define REGISTER_CIRCUIT_H

#include <stdint.h>
#include "Circuit.h"
#include "Pin.h"

class RegisterCircuit final : public Circuit
{
public:
	RegisterCircuit();
	RegisterCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_rReg1;
	InputPin m_rReg2;
	InputPin m_wReg;
	InputPin m_wData;

	// clock signal
	InputPin m_clockInput;

	// control signal
	InputPin m_regWrite;

	OutputPin m_readData1;
	OutputPin m_readData2;

	bool m_outBuffuer1[64];
	bool m_outBuffer2[64];

	uint32_t m_registers[32];
};

#endif
