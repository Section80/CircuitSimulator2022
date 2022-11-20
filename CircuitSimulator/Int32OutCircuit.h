#ifndef INT32_OUT_CIRCUIT
#define INT32_OUT_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class Int32OutCircuit final : public Circuit
{
public:
	Int32OutCircuit();
	Int32OutCircuit(float x, float y);

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	inline int GetValue() { return m_val; }
	void SetValue(int value);

private:
	void Render() override;

private:
	void updateOutput() override;

private:
	OutputPin m_outputPin;

	int m_textInputId;

	int m_val;

	bool m_outBuf1[32];
	bool m_outBuf2[32];
};

#endif
