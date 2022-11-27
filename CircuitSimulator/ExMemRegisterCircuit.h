#ifndef EX_MEM_REGISTER_CIRCUIT_H
#define EX_MEM_REGISTER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class ExMemRegisterCircuit final : public Circuit
{
public:
	ExMemRegisterCircuit();
	ExMemRegisterCircuit(float x, float y);

	void Render() override;
	void RenderWire(bool bSummary = false) override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	// WB
	InputPin m_regWrite_in;
	InputPin m_memToReg_in;
	// MEM
	InputPin m_memRead_in;
	InputPin m_memWrite_in;
	/* ================ */
	InputPin m_aluResult_in;
	InputPin m_read2_in;
	InputPin m_writeReg_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	// WB
	OutputPin m_regWrite_out;		// 1bit : [72, 72]
	OutputPin m_memToReg_out;		// 1bit : [71, 71]
	// MEM
	OutputPin m_memRead_out;		// 1bit : [70, 70]
	OutputPin m_memWrite_out;		// 1bit : [69, 69]
	/* ================ */
	OutputPin m_aluResult_out;		// 32bit : [37, 68]
	OutputPin m_read2_out;			// 32bit : [5, 36]
	OutputPin m_writeReg_out;		// 5bit : [0, 4]

	bool m_outBuf1[73];
	bool m_outBuf2[73];

	uint32_t m_data[10];
};

#endif