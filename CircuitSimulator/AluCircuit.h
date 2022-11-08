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
	InputPin m_in1;
	InputPin m_in2;
	InputPin m_operation;

	OutputPin m_out;
	OutputPin m_zero;

	bool m_outBuf1[33];
	bool m_outBuf2[33];
};

enum class EAluOperation
{
	And = 0,
	Or = 1,
	Add = 2,
	Sub = 6, 
	Slt = 7,
	Nor = 12
};

class AluOperationCircuit final : public Circuit {
public:
	AluOperationCircuit();
	AluOperationCircuit(float x, float y);

private:
	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	OutputPin m_out;

	bool m_outBuf1[4];
	bool m_outBuf2[4];

	EAluOperation m_op;
	int m_buttonId;
};


#endif
