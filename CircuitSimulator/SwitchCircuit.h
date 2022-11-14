#ifndef SWITCH_CIRCUIT_H
#define SWITCH_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class SwitchCircuit final : public Circuit
{
public:
	SwitchCircuit();
	SwitchCircuit(float x, float y);

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	void SetPressed(bool pressed);
	inline bool GetPressed() { return m_val; }

private:
	void render() override;

private:
	void updateOutput() override;

private:
	OutputPin m_outputPin;

	int m_buttonId;

	bool m_val;

	bool m_outBuf1;
	bool m_outBuf2;
};

#endif
