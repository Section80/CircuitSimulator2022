#ifndef PROGRAM_COUNTER_CIRCUIT_H
#define PROGRAM_COUNTER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class ProgramCounterCircuit final : public Circuit
{
public:
	ProgramCounterCircuit();
	ProgramCounterCircuit(float x, float y);

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	inline void SetAddress(int address) 
	{ 
		m_addr = address; 
		updateOutput();
		afterUpdateOutput();
	}

public:
	static ProgramCounterCircuit* Instance;

private:
	void updateOutput() override;

private:
	InputPin m_inAddr;
	
	InputPin m_write;
	
	InputPin m_clock;
	bool m_bLastClock;

	OutputPin m_outAddr;

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	uint32_t m_addr;
};

#endif