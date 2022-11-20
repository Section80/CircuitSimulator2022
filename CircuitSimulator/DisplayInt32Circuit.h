#ifndef DISPLAY_INT32_CIRCUIT
#define DISPLAY_INT32_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class DisplayInt32Circuit final : public Circuit
{
public:
	DisplayInt32Circuit();
	DisplayInt32Circuit(float x, float y);

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void Render() override;

private:
	InputPin m_inputPin;

	char m_buffer[32];
};

#endif
