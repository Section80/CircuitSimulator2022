#ifndef EX_MEM_REGISTER_CIRCUIT_H
#define ID_EX_REGISTER_CIRCUIT_H


#include "Circuit.h"
#include "Pin.h"

class IdExRegisterCircuit final : public Circuit
{
public:
	IdExRegisterCircuit();
	IdExRegisterCircuit(float x, float y);

	void Render() override;
	void RenderWire(bool bSummary = false) override;
	void RenderInspector() override;

	InputPin* GetInputPin(int index) override;
	OutputPin* GetOutputPin(int index) override;

	std::string CurrentInstruction;

public:
	static IdExRegisterCircuit* Instance;

private:
	void updateOutput() override;

private:
	// WB
	InputPin m_regWrite_in;		// whether to write register file or not
	InputPin m_memToReg_in;		// from alu result(0) or data memory(1)
	// MEM
	InputPin m_memRead_in;
	InputPin m_memWrite_in;
	// EX
	InputPin m_regDest_in;		// rt(0) or rd(1) for write register
	InputPin m_aluOp_in;
	InputPin m_aluSrc_in;			// use read data2(0) or sign-extended(1)
	/* ================ */
	InputPin m_read1_in;
	InputPin m_read2_in;
	InputPin m_signExtended_in;
	InputPin m_rs_in;
	InputPin m_rt_in;
	InputPin m_rd_in;

	// clock signal
	InputPin m_clock;
	bool m_bLastClock;
	
	// WB
	OutputPin m_regWrite_out;		// 1bit : [118, 118]
	OutputPin m_memToReg_out;		// 1bit : [117, 117]
	// MEM
	OutputPin m_memRead_out;		// 1bit : [116, 116]
	OutputPin m_memWrite_out;		// 1bit : [115, 115]
	// EX
	OutputPin m_regDest_out;		// 1bit : [114, 114]
	OutputPin m_aluOp_out;			// 2bit : [112, 113]
	OutputPin m_aluSrc_out;			// 1bit : [111, 111]
	/* ================ */
	OutputPin m_read1_out;			// 32bit : [79, 110]
	OutputPin m_read2_out;			// 32bit : [47, 78]
	OutputPin m_signExtended_out;	// 32bit : [15, 46]
	OutputPin m_rs_out;				// 5bit : [10, 14]
	OutputPin m_rt_out;				// 5bit : [5, 9]
	OutputPin m_rd_out;				// 5bit : [0, 4]

	bool m_outBuf1[119];
	bool m_outBuf2[119];

	/* =============== CAUTION ===============
	* 회로를 업데이트하면서 outoputPin 수를 하나 늘렸다. 
	* 그런데 아래 m_data 배열의 크기를 늘리는 것을 까먹었다. 
	* 15가 아닌 14로 설정했다. 
	* 그러자 레지스터에 538976288라는 무지막지한 숫자가 나왔다. 
	* 왜 그랬을까? m_data 배열을 참조할 때 인덱스를 오버런해서 
	* m_strBuf를 참조했기 때문이라고 추정된다. 
	* 인스펙터를 렌더링하기 위해 sprintf()를 했을 때
	* 출력할 문자열로 값이 써진다. 디버깅하는 중에는 마치 랜덤하게 
	* 발생하는 것 처럼 느껴졌다. 실제로 나는 30번에 한번 정도
	* 발생한다고 생각했다. m_data 배열의 크기를 14로 줄인다고 
	* 해당 버그가 항상 발생하지는 않는다. imgui의 렌더링 순서에 
	* 따라 다른 듯 하다. 확실히 하기 위해 14로 계속 설정해 둔 
	* 뒤, 해당 값이 m_strBuf의 하위 32비트와 일치하는지 확인할 
	* 필요가 있다. 
	* 538976288은 hex로 0x20202020, 
	* binary로 0b100000001000000010000000100000이다. 
	* 규칙성이 있는 것을 보니 스트링 값은 아닌듯 하다. 
	* 8비트 1이 연속돼어있는 듯 한... 
	* 해당 값으 출력한 회로는 2:! 먹스였고, 해당 값 입력은 
	* SignExtension에 연결되어 있었다. 
	* 그렇다면 말이 된다. 왜냐면 SignExtension은 index 15의 값을
	* 상위 비트 바이트 단위로 반복해서 복사하니까.. 말이 된다. 
	 */
	uint32_t m_data[15];
	// char m_strBuf[256]; // 사용하지 않음
};

#endif
