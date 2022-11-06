#ifndef REGISTER_INDEX_OUT_CIRCUIT
#define REGISTER_INDEX_OUT_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class RegIdxOutCircuit final : public Circuit
{
public:
	RegIdxOutCircuit();
	RegIdxOutCircuit(float x, float y);

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void render() override;

private:
	OutputPin m_outputPin;

	int m_textInputId;

	unsigned int m_val;

	bool m_outBuf1[5];
	bool m_outBuf2[5];
};

#endif
