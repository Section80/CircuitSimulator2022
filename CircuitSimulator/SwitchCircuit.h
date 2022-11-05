#ifndef SWITCH_CIRCUIT_H
#define SWITCH_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class SwitchCircuit final : public Circuit
{
public:
	SwitchCircuit();

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void render() override;

private:
	void updateOutput() override;

private:
	OutputPin m_outputPin;

	int m_buttonId;

	bool m_val;

	bool m_outBuf1;
	bool m_outBuf2;
};

#endif
