#ifndef IF_ID_REGISTER_CIRCUIT_H
#define IF_ID_REGISTER_CIRCUIT_H


#include "Circuit.h"
#include "Pin.h"

class IfIdRegisterCircuit final : public Circuit
{
public:
	IfIdRegisterCircuit();
	IfIdRegisterCircuit(float x, float y);

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
