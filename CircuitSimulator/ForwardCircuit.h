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
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_rs;				// 5-bit
	InputPin m_rt;				// 5-bit
	InputPin m_exMemAluReg;		// 5-bit
	InputPin m_exMemRegWrite;	// 1-bit
	InputPin m_memWbAluReg;		// 5-bit
	InputPin m_memWbRegWrite;	// 1-bit

	OutputPin m_forwardA;		// 2-bit: [2 : 3]
	OutputPin m_forwardB;		// 2-bit: [0 : 1]

	bool m_outBuf1[4];
	bool m_outBuf2[4];

	int m_iForwardA;
	int m_iForwardB;
};


#endif