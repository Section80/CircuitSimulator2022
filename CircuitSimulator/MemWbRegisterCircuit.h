#ifndef MEM_WB_REGISTER_CIRCUIT_H
#define MEM_RB_REGISTER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class MemWbRegisterCircuit final : public Circuit
{
public:
	MemWbRegisterCircuit();
	MemWbRegisterCircuit(float x, float y);

	void Render() override;
	void RenderWire(bool bSummary = false) override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	std::string CurrentInstruction;

public:
	static MemWbRegisterCircuit* Instance;

private:
	void updateOutput() override;

private:
	// WB
	InputPin m_regWrite_in;
	InputPin m_memToReg_in;
	/* ================ */
	InputPin m_readData_in;
	InputPin m_aluResult_in;
	InputPin m_writeReg_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	// WB
	OutputPin m_regWrite_out;		// 1bit : [70, 70]
	OutputPin m_memToReg_out;		// 1bit : [69, 69]
	/* ================ */
	OutputPin m_readData_out;		// 32bit : [37, 68]
	OutputPin m_aluResult_out;		// 32bit : [5, 36]
	OutputPin m_writeReg_out;		// 5bit : [0, 4]

	bool m_outBuf1[71];
	bool m_outBuf2[71];

	uint32_t m_data[5];
};

#endif
