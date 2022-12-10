#ifndef HAZARD_DETECTION_UNIT_H
#define HAZARD_DETECTION_UNIT_H

#include "Circuit.h"
#include "Pin.h"

// Branch Hazard ³»¿ë: 321p, 359p

class HazardDetectionUnit final : public Circuit
{
public:
	HazardDetectionUnit();
	HazardDetectionUnit(float x, float y);

	void Render() override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

private:
	void updateOutput() override;

private:
	InputPin m_branch;
	InputPin m_jump;
	InputPin m_memWrite;

	InputPin m_IFID_rs;
	InputPin m_IFID_rt;
	
	InputPin m_IDEX_memRead;
	InputPin m_IDEX_regWrite;
	InputPin m_IDEX_writeReg;
	
	InputPin m_EXMEM_memRead;
	InputPin m_EXMEM_regWrite;
	InputPin m_EXMEM_writeReg;

	OutputPin m_stall;		// 1bit
	OutputPin m_forwardA;	// 1bit
	OutputPin m_forwardB;	// 1bit

	bool m_outBuf1[3];
	bool m_outBuf2[3];
};

#endif