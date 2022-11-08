#ifndef ALU_CIRUIT_H
#define ALU_CIRUIT_H

#include "Circuit.h"
#include "Pin.h"

class AluCircuit final : public Circuit
{
public:
	AluCircuit();
	AluCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;


private:

};

#endif
