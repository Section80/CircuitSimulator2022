#ifndef IF_ID_REGISTER_CIRCUIT_H
#define IF_ID_REGISTER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class IfIdRegisterCircuit final : public Circuit
{
public:
	IfIdRegisterCircuit();
	IfIdRegisterCircuit(float x, float y);

	void Render() override;
	void RenderWire(bool bSummary = false) override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	std::string CurrentInstruction;

public:
	static IfIdRegisterCircuit* Instance;

private:
	void updateOutput() override;

	// 각 스테이지에서 실행하고 있는 명령어 표기 업데이트
	void updateInstructionForEachStage();

private:
	InputPin m_pc_in;				// 32bit
	InputPin m_instruction_in;		// 32bit
	InputPin m_write;				// 1bit
	InputPin m_flush;				// 1bit

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;

	OutputPin m_pc_out;				// 32bit : [32, 63]
	OutputPin m_instruction_out;	// 32bit : [0, 31]
	OutputPin m_rs_out;				//  5bit : [21, 25]
	OutputPin m_rt_out;				//  5bit : [16, 20]
	OutputPin m_rd_out;				//  5bit : [11, 15]
	OutputPin m_low16_out;			// 16bit : [ 0 ,15]
	OutputPin m_addr_out;			// 26bit : [ 0, 25]

	bool m_outBuf1[64];
	bool m_outBuf2[64];

	uint32_t m_pc;
	uint32_t m_instruction;
};

#endif
