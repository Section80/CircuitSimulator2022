#ifndef INSTRUCTION_MEMORY_H
#define INSTRUCTION_MEMORY_H

#include <unordered_map>
#include "Circuit.h"
#include "Pin.h"

class InstructionMemoryCircuit final : public Circuit
{
public:
	InstructionMemoryCircuit();
	InstructionMemoryCircuit(float x, float y);
	virtual ~InstructionMemoryCircuit();
	
	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_addr;
	
	OutputPin m_rs;		// 5bit
	OutputPin m_rt;		// 5bit
	OutputPin m_rd;		// 5bit
	OutputPin m_low16;	// 16bit

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	std::unordered_map<int, int> m_map;

	int m_val;
};

#endif