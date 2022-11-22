#ifndef INSTRUCTION_MEMORY_H
#define INSTRUCTION_MEMORY_H

#include <map>
#include "Circuit.h"
#include "Pin.h"

class InstructionMemoryCircuit final : public Circuit
{
public:
	InstructionMemoryCircuit();
	InstructionMemoryCircuit(float x, float y);
	virtual ~InstructionMemoryCircuit();
	
	void Render() override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	void LoadInstruction(std::string path);
	inline std::string& GetPath() { return m_path; }

private:
	void updateOutput() override;

private:
	InputPin m_addr;
	OutputPin m_out;	// instruction

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	std::map<int, int> m_map;

	int m_val;
	int m_currentOutAddr;
	int m_loadButtonId;

	std::string m_path;
};

#endif