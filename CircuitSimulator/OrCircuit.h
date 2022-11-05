#ifndef OR_CIRCUIT
#define OR_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class OrCircuit final : public Circuit
{
public:
	OrCircuit();

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


class NorCircuit final : public Circuit
{
public:
	NorCircuit();

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
