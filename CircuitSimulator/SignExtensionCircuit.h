#ifndef SIGN_EXTENSION_CIRCUIT
#define SIGN_EXTENSION_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class SignExtensionCircuit final : public Circuit
{
public:
	SignExtensionCircuit();
	SignExtensionCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
};

#endif
