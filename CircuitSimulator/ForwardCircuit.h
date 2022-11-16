#ifndef FORWARD_UNIT_CIRCUIT_H
#define FORWARD_UNIT_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class ForwardCircuit final : public Circuit
{
public:
	ForwardCircuit();
	ForwardCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_rt;
	InputPin m_rs;
	InputPin m_exMemAluReg;
	InputPin m_exMemRegwrite;
	InputPin m_memWbAluReg;
	InputPin m_memWbRegWrite;

	OutputPin m_forwardA;
	OutputPin m_forwardB;

	bool m_outBuf1[4];
	bool m_outBuf2[4];
};


#endif