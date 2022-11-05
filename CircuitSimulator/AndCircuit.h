#ifndef AND_CIRCUIT_H
#define AND_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class AndCircuit final : public Circuit
{
public: 
	AndCircuit(float delay);

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_inputPin0;
	InputPin m_inputPin1;
	OutputPin m_outputPin;

	bool m_outputBuffer1;
	bool m_outputBuffer2;
};

#endif
