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
	* ȸ�θ� ������Ʈ�ϸ鼭 outoputPin ���� �ϳ� �÷ȴ�. 
	* �׷��� �Ʒ� m_data �迭�� ũ�⸦ �ø��� ���� ��Ծ���. 
	* 15�� �ƴ� 14�� �����ߴ�. 
	* �׷��� �������Ϳ� 538976288��� ���������� ���ڰ� ���Դ�. 
	* �� �׷�����? m_data �迭�� ������ �� �ε����� �������ؼ� 
	* m_strBuf�� �����߱� �����̶�� �����ȴ�. 
	* �ν����͸� �������ϱ� ���� sprintf()�� ���� ��
	* ����� ���ڿ��� ���� ������. ������ϴ� �߿��� ��ġ �����ϰ� 
	* �߻��ϴ� �� ó�� ��������. ������ ���� 30���� �ѹ� ����
	* �߻��Ѵٰ� �����ߴ�. m_data �迭�� ũ�⸦ 14�� ���δٰ� 
	* �ش� ���װ� �׻� �߻������� �ʴ´�. imgui�� ������ ������ 
	* ���� �ٸ� �� �ϴ�. Ȯ���� �ϱ� ���� 14�� ��� ������ �� 
	* ��, �ش� ���� m_strBuf�� ���� 32��Ʈ�� ��ġ�ϴ��� Ȯ���� 
	* �ʿ䰡 �ִ�. 
	* 538976288�� hex�� 0x20202020, 
	* binary�� 0b100000001000000010000000100000�̴�. 
	* ��Ģ���� �ִ� ���� ���� ��Ʈ�� ���� �ƴѵ� �ϴ�. 
	* 8��Ʈ 1�� ���ӵž��ִ� �� ��... 
	* �ش� ���� ����� ȸ�δ� 2:! �Խ�����, �ش� �� �Է��� 
	* SignExtension�� ����Ǿ� �־���. 
	* �׷��ٸ� ���� �ȴ�. �ֳĸ� SignExtension�� index 15�� ����
	* ���� ��Ʈ ����Ʈ ������ �ݺ��ؼ� �����ϴϱ�.. ���� �ȴ�. 
	 */
	uint32_t m_data[15];
	// char m_strBuf[256]; // ������� ����
};

#endif
