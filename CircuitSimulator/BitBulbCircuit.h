#ifndef BIT_BULB_CIRCUIT_H
#define BIT_BULB_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class BitBulbCircuit final : public Circuit
{
public:
	BitBulbCircuit();

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput();

private:
	InputPin m_inputPin;
};

#endif
