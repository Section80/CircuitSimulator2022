#ifndef AND_CIRCUIT_H
#define AND_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class AndCircuit final : public Circuit
{
public: 
	AndCircuit();
	AndCircuit(float x, float y);

	void render() override;

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

class NandCircuit final : public Circuit
{
public:
	NandCircuit();
	NandCircuit(float x, float y);

	void render() override;

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
