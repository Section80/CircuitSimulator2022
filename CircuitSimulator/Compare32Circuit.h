#ifndef COMPARE32_CIRCUIT_H
#define COMPARE32_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class Compare32Circuit final : public Circuit
{
public: 
	Compare32Circuit();
	Compare32Circuit(float x, float y);

	void Render() override;
	// void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private: 
	InputPin m_in1;		// 32 bit
	InputPin m_in2;		// 32 bit

	OutputPin m_equal;	// 1bit

	bool m_outBuf1;
	bool m_outBuf2;
};

#endif
