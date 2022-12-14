#ifndef CLOCK_CIRCUIT_H
#define CLOCK_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class ClockCircuit final : public Circuit
{
public:
	ClockCircuit();
	ClockCircuit(float x, float y);

	void Render() override;
	void RenderWire(bool bSummary = false) override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	inline void ResetDelay() { resetDelay(); };

public:
	static ClockCircuit* Instance;

protected:
	void afterUpdateOutput() override;

private:
	void updateOutput() override;

private:
	OutputPin m_out;

	bool m_outBuf1;
	bool m_outBuf2;

	bool m_bVal;
};


#endif 