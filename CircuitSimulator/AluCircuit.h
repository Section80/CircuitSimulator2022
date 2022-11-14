#ifndef ALU_CIRUIT_H
#define ALU_CIRUIT_H

#include "Circuit.h"
#include "Pin.h"

enum class EAluOperation
{
	And = 0b0000,
	Or = 0b0001,
	Add = 0b0010,
	Sub = 0b0110,
	Slt = 0b0111,
	Nor = 0b1100
};

class AluCircuit final : public Circuit
{
public:
	AluCircuit();
	AluCircuit(float x, float y);

	void render() override;
	void RenderInspector() override;

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

	char m_strBuf[256];

	EAluOperation m_lastOp;
	int m_lastResult = 0;
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
