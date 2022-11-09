#ifndef ALU_CONTROL_CIRCUIT_H
#define ALU_CONTROL_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class AluControlCircuit final : public Circuit
{
public:
	AluControlCircuit();
	AluControlCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_op;			// 2-bit operaion from Control Signal
	InputPin m_funct;

	OutputPin m_control;	// 4-bit ALU Control Signal

	bool m_outBuf1[4];
	bool m_outBuf2[4];
};

#endif
