#ifndef MUX_CIRCUIT
#define MUX_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class MuxCircuit final : public Circuit
{
public:
	MuxCircuit();
	MuxCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;


private:

};

#endif
