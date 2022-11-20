#ifndef HAZARD_DETECTION_UNIT_H
#define HAZARD_DETECTION_UNIT_H

#include "Circuit.h"
#include "Pin.h"

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
	InputPin m_ifIdRs;
	InputPin m_ifIdRt;
	InputPin m_idExMemRead;
	InputPin m_idExWriteReg;

	OutputPin m_pcWrite;
	OutputPin m_ifIdWrite;
	OutputPin m_select;

	bool m_outBuf1[3];
	bool m_outBuf2[3];

	bool m_bStall;
};

#endif