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
	// clock signal
	InputPin m_clock;
	bool m_bLastClock;
};

#endif
