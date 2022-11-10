#include "EdgeSenstiveCircuit.h"

EdgeSenstiveCircuit::EdgeSenstiveCircuit(
	const char* name, 
	int inputPinCount, int outputPinCount, 
	bool* outputBuffer1, bool* outputBuffer2, 
	int outputSize, float delay, 
	int clockIndex, EClockEdge clockEdge)
	: Circuit(name, inputPinCount, outputPinCount, outputBuffer1, outputBuffer2, outputSize, delay)
	, m_clockIndex(clockIndex)
	, m_bLastClock(false)
	, m_clockEdge(clockEdge)
{}

void EdgeSenstiveCircuit::onInputChanged()
{
	bool bClock = GetInputPin(m_clockIndex)->ReadAt(0);
	if (
		bClock && !m_bLastClock ||	// rising edge
		!bClock == m_bLastClock		// clock 말고 다른 입력이 변한 경우
		)
	{

	}

	m_bLastClock = bClock;
}
