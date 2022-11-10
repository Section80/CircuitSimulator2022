#ifndef EDGE_SENSTIVE_CIRCUIT_H
#define EDGE_SENSTIVE_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

enum class EClockEdge
{
	Rising,
	Falling,
	Double
};

class EdgeSenstiveCircuit abstract : public Circuit
{
public:
	EdgeSenstiveCircuit(
		const char* name,
		int inputPinCount, int outputPinCount,
		bool* outputBuffer1, bool* outputBuffer2,
		int outputSize,
		float delay, int clockIndex,
		EClockEdge clockEdge = EClockEdge::Rising
	);

	virtual void render() override = 0;

	virtual InputPin* GetInputPin(int index) override = 0;
	virtual OutputPin* GetOutputPin(int index) override = 0;

private:
	virtual void updateOutput() override = 0;

	void onInputChanged() override;

private:
	int m_clockIndex;
	bool m_bLastClock;

	EClockEdge m_clockEdge;
};

#endif