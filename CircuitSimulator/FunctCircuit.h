#ifndef FUNT_CIRCUIT_H
#define FUNT_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class FunctCircuit final : public Circuit
{
public:
	FunctCircuit();
	FunctCircuit(float x, float y);

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_signEx;
	OutputPin m_funct;

	bool m_outBuf1[6];
	bool m_outBuf2[6];
};

#endif