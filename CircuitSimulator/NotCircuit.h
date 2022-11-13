#ifndef NOT_CIRCUIT_H
#define NOT_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class NotCircuit final : public Circuit
{
public:
	NotCircuit();
	NotCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_inputPin;
	OutputPin m_outputPin;

	bool m_outBuf1;
	bool m_outBuf2;
};

#endif
