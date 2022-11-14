#ifndef BUFFER_CIRCUIT_H
#define BUFFER_CIRCUIT_H

#include "Circuit.h"
#include "Pin.h"

class BufferCircuit final : public Circuit
{
public:
	BufferCircuit();
	BufferCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	void SetWireLineCount(int count);
	inline int GetWireLineCount() { return m_wireLineCount; }
	inline void SetIsReversed(bool isReversed) { m_reverse = isReversed; }
	inline bool IsReversed() { return m_reverse; }
	
private:
	void updateOutput() override;

private:
	InputPin m_inputPin;
	OutputPin m_outputPin;

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	bool m_reverse;
	int m_wireLineCount;
	int m_inputIntId;
	int m_intInputBuffer;
	int m_swapButtonId;
};


class ClockBufferCircuit final : public Circuit
{
public:
	ClockBufferCircuit();
	ClockBufferCircuit(float x, float y);

	void render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	inline void SetIsReversed(bool isReversed) { m_reverse = isReversed; }
	inline bool IsReversed() { return m_reverse; }

private:
	void updateOutput() override;

	// 원래는 출력이 변했을 때만 연결된 회로를 깨운다. 
	// 그런데 clock은 rising edge에만 출력을 전파하기 때문에 
	// 일반 버퍼를 사용하면 클럭 시그널이 전파가 안된다. 
	// 그래서 출력이 변하지 않았을 때도 연결되 회로를 깨우게 한다. 
	void afterUpdateOutput() override;

private:
	InputPin m_inputPin;
	OutputPin m_outputPin;

	bool m_outBuf1[1];
	bool m_outBuf2[1];

	bool m_reverse;
	int m_swapButtonId;
};

#endif
