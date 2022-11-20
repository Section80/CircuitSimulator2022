#ifndef SIGN_EXTENSION_CIRCUIT
#define SIGN_EXTENSION_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class SignExtensionCircuit final : public Circuit
{
public:
	SignExtensionCircuit();
	SignExtensionCircuit(float x, float y);

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
