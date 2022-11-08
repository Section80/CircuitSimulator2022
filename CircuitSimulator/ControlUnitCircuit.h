#ifndef CONTROL_UNIT_CIRCUIT
#define CONTROL_UNIT_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class ControlUnitCircuit final : public Circuit
{
public:
	ControlUnitCircuit();
	ControlUnitCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;


private:

};

#endif
