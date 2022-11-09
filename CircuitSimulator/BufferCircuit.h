#ifndef BUFFER_CIRCUIT_H
#define BUFFER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class BufferCircuit final : public Circuit
{
public:
	BufferCircuit();
	BufferCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	void SetWireLineCount(int count);
	
private:
	void updateOutput() override;

private:
	InputPin m_inputPin;
	OutputPin m_outputPin;

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	bool m_reverse;
	int m_wireLineCount;
	int m_inputIntId;
};

#endif
