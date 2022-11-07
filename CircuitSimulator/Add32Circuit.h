#ifndef AND32_CIRCUIT_H
#define AND32_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class Add32Circuit : public Circuit
{
public:
	Add32Circuit();
	Add32Circuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_in1;
	InputPin m_in2;

	OutputPin m_out;

	bool m_outBuf1[32];
	bool m_outBuf2[32];
};

#endif
