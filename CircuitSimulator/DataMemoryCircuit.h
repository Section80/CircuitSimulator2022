#ifndef MEMORY_CIRCUIT_H
#define MEMORY_CIRCUIT_H

#include <map>	// iteration 할 때 메모리 순으로 정렬
#include "Circuit.h"
#include "Pin.h"
#include "DataMemory.h"

class DataMemoryCircuit final : public Circuit
{
public:
	DataMemoryCircuit();
	DataMemoryCircuit(float x, float y);

	void Render() override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	void LoadData(std::string path);
	inline std::string& GetPath() { return m_path; }

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

	int m_lastRData;
	OutputPin m_rData;

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	int m_loadButtonId;
	int m_lastChanged;

	DataMemory m_dataMemory;
	std::string m_path;
};

#endif
