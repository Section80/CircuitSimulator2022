#ifndef MUX_CIRCUIT
#define MUX_CIRCUIT

#include "Circuit.h"
#include "Pin.h"

class Mux21Circuit final : public Circuit
{
public:
	Mux21Circuit();
	Mux21Circuit(float x, float y);

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	inline int GetWireLineCount() { return m_wireLineCount; }
	void SetWireLineCount(int count);

private:
	void updateOutput() override;

private:
	InputPin m_in0;
	InputPin m_in1;
	InputPin m_select;

	OutputPin m_out;

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	int m_intInputId;
	int m_intInputBuffer;
	int m_wireLineCount;
};


class Mux31Circuit final : public Circuit
{
public:
	Mux31Circuit();
	Mux31Circuit(float x, float y);

	void Render() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	inline int GetWireLineCount() { return m_wireLineCount; }
	void SetWireLineCount(int count);

private:
	void updateOutput() override;

private:
	InputPin m_in0;
	InputPin m_in1;
	InputPin m_in2;
	InputPin m_select;

	OutputPin m_out;

	bool m_outBuf1[32];
	bool m_outBuf2[32];

	int m_intInputId;
	int m_intInputBuffer;
	int m_wireLineCount;
};



#endif
