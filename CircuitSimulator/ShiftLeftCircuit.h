#ifndef SHIFT_LEFT_CIRCUIT_H
#define SHIFT_LEFT_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class ShiftLeftCircuit final : public Circuit
{
public:
	ShiftLeftCircuit();
	ShiftLeftCircuit(float x, float y);

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_in;
	OutputPin m_out;

	bool m_outBuf1[32];
	bool m_outBuf2[32];
};


#endif
