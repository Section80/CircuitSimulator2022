#include "stdafx.h"
#include "ExMemRegisterCircuit.h"

void ExMemRegisterCircuit::updateOutput()
{
	bool bClock = m_clock.ReadAt(0);
	bool bRisingEdge = false;
	bool bFallingEdge = false;
	if (m_bLastClock != bClock)
	{
		if (bClock)
		{
			// rising edge
			bRisingEdge = true;
		}
		else
		{
			// falling edge
			bFallingEdge = true;
		}
	}
	m_bLastClock = bClock;

	if (bRisingEdge)
	{
		if (false)
		{
			// update edge triggred part here

			// �Է��� ������ �ʴ��� ����� ������Ʈ�ϵ���
			// ���� �����̸� ���½�Ų��. 
			resetDelay();
		}
	}
}
