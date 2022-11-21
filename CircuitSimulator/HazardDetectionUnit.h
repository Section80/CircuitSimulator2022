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
	InputPin m_IFID_rs;
	InputPin m_IFID_rt;
	InputPin m_IDEX_memRead;
	InputPin m_IDEX_writeReg;
	InputPin m_EXMEM_memWrite;
	InputPin m_EXMEM_writeReg;

	OutputPin m_pcWrite;
	OutputPin m_ifIdWrite;
	OutputPin m_selectCtrl;
	OutputPin m_selectRead1;

	bool m_outBuf1[4];
	bool m_outBuf2[4];

	bool m_bStall;
	bool m_bSaveUsed2;
};

#endif