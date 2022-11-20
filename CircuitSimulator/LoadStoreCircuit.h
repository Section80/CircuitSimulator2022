#ifndef LOAD_STORE_CIRCUIT_H
#define LOAD_STORE_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class LoadStoreCircuit final : public Circuit
{
public:
	LoadStoreCircuit();
	LoadStoreCircuit(float x, float y);

	void Render() override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_memWbRegWrite;
	InputPin m_exMemWriteReg;
	InputPin m_memWbWriteReg;

	bool m_outBuf1;
	bool m_outBuf2;

	OutputPin m_select;
};

#endif