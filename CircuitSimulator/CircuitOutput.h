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

	// OldBuffer�� CurrentBuffer�� ��ȯ�Ѵ�. 
	void Swap();
	// CurrentBuffer���� �д´�. 
	bool ReadAt(int index) override;
	// CurrentBuffer�� ����. 
	void Write(const bool* pData, int offset, int size);

	inline bool* GetCurrentBuffer() { return m_pCurrentBuffer; }

	// OldBuffer, CurrentBuffer���� offset���� len ��ŭ�� ���� ���ߴ��� Ȯ���Ѵ�. 
	bool IsChanged(int offset, int len);
	// �б� ���� �������̽��� ĳ�����Ѵ�. 
	inline IReadCircuitOutput* GetReader() { return (IReadCircuitOutput*)this; }

private:
	bool* m_pOldBuffer;
	bool* m_pCurrentBuffer;
	int m_size;
};

#endif
