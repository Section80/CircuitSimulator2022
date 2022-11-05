#ifndef BUFFER_CIRCUIT_H
#define BUFFER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class BufferCircuit final : public Circuit
{
public:
	BufferCircuit();

	void Render() override;

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
