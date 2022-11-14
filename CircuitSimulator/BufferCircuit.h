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

	// ������ ����� ������ ���� ����� ȸ�θ� �����. 
	// �׷��� clock�� rising edge���� ����� �����ϱ� ������ 
	// �Ϲ� ���۸� ����ϸ� Ŭ�� �ñ׳��� ���İ� �ȵȴ�. 
	// �׷��� ����� ������ �ʾ��� ���� ����� ȸ�θ� ����� �Ѵ�. 
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
