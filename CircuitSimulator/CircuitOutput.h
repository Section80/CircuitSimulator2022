#ifndef CIRCUIT_OUTPUT_H
#define CIRCUIT_OUTPUT_H

class IReadCircuitOutput abstract
{
public:
	virtual bool ReadAt(int index) = 0;
};

class CircuitOutput final : public IReadCircuitOutput
{
public:
	CircuitOutput(bool* pBuffer1, bool* pBuffer2, int size);

	// OldBuffer와 CurrentBuffer를 교환한다. 
	void Swap();
	// CurrentBuffer에서 읽는다. 
	bool ReadAt(int index) override;
	// CurrentBuffer에 쓴다. 
	void Write(const bool* pData, int offset, int size);

	inline bool* GetCurrentBuffer() { return m_pCurrentBuffer; }

	// OldBuffer, CurrentBuffer에서 offset부터 len 만큼의 값이 변했는지 확인한다. 
	bool IsChanged(int offset, int len);
	// 읽기 전용 인터페이스로 캐스팅한다. 
	inline IReadCircuitOutput* GetReader() { return (IReadCircuitOutput*)this; }

private:
	bool* m_pOldBuffer;
	bool* m_pCurrentBuffer;
	int m_size;
};

#endif
