#ifndef JUMP_ADDRESS_CIRCUIT_H
#define JUMP_ADDRESS_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class JumpAddressCircuit final : public Circuit
{
public:
	JumpAddressCircuit();
	JumpAddressCircuit(float x, float y);

	void Render() override;
	// void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_pc;
	InputPin m_address;

	OutputPin m_jump;

	bool outBuf1[32];
	bool outBuf2[32];
};

#endif