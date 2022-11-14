#ifndef MEMORY_CIRCUIT_H
#define MEMORY_CIRCUIT_H

#include <map>	// iteration 할 때 메모리 순으로 정렬
#include "Circuit.h"
#include "Pin.h"

class DataMemoryCircuit final : public Circuit
{
public:
	DataMemoryCircuit();
	DataMemoryCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_addr;
	InputPin m_wData;

	// control signal
	InputPin m_memRead;
	InputPin m_memWrite;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	OutputPin m_rData;

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	std::map<int, int> m_data;

	int m_loadButtonId;
};

#endif
