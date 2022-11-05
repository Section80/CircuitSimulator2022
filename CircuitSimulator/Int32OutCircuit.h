#ifndef INT32_OUT_CIRCUIT
#define INT32_OUT_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class Int32OutCircuit final : public Circuit
{
public:
	Int32OutCircuit();

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void render() override;

private:
	void updateOutput() override;

private:
	static int inputTextCallback(ImGuiInputTextCallbackData* pData);

private:
	OutputPin m_outputPin;

	int m_textInputId;

	int m_val;

	bool m_outBuf1[32];
	bool m_outBuf2[32];
};

#endif
