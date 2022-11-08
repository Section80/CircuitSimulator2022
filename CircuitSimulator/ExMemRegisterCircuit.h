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
	// clock signal
	InputPin m_clock;
	bool m_bLastClock;
};

#endif