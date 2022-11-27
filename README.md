# MIPS Simulator #

실시간으로 프로세서의 각 회로의 딜레이를 시물레이션하는 MIPS Simluator

기능: 
 - GUI를 통한 회로 편집
 - 회로의 Delay 표기
 - 회로 Inspector를 통한 상태 등 정보 표기
 - MIPS 명령어(and/or/add/sub/slt/lw/sw/beq/j) 수행
 - 실시간/사이클 단위 명령어 수행

# 코드 설명 #

새로운 회로를 추가하기 편하도록 클래스를 설계했다.  모든 회로들은 Circuit 클래스를 상속받아 공통된 로직은 Circuit 클래스에서 처리한다.

각 회로들은 회로 자체의 딜레이를 나타내는 m_delay와 현재 남은 delay를 나타내는 m_leftDelay 멤버를 가지고 있다. m_delay가 0인 경우, 입력 데이터가 변하자마자 해당 회로의 출력이 바로 변하고, 출력에 연결된 회로들에게 전파된다. 

m_delay가 0이 아닌 경우, 입력 데이터가 변하면 m_leftDelay의 값이 m_delay로 초기화된다. m_leftDelay의 값은 메인 루프에서 계속 호출되는 Circuit::UpdateAll(double dt) 함수에서 지난 시간만큼 감소되며, 0이 된 경우 해당 회로의 출력 업데이트-전파가 일어난다. 

### Circuit 클래스의 주요 함수

다음은 Circuit들의 공통된 로직을 처리하는 주요 함수들이다. 

#### onInputChanged()

어떤 회로의 출력이 변했을 때 해당 회로는 출력에 연결된 회로들의 onInputChanged() 함수를 호출한다. 
``` c++
void Circuit::onInputChanged()
{
	// Instance Circuit인 경우
	if (m_delay == 0)
	{
		m_circuitOutput.Swap();
		// 출력이 업데이트되고 바로 전파된다. 
		updateOutput();
		afterUpdateOutput();
	}
	else {
		// delay를 초기화한다. 
		m_leftDelay = m_delay;
	}
}
```
m_delay가 0인 경우에는 해당 회로의 출력을 바로 업데이트한 뒤 변경된 출력을 출력 와이어에 연결된 회로들에게 전파한다.  
출력을 업데이트한 뒤 출력 와이어의 값이 변하지 않았을 수도 있다. 그래서 이전 출력을 m_circuitOutput 저장해놓고(더블 버퍼 스왑) 현재 출력과 비교해 변화한 경우에만 바뀐 경우에만 새로운 출력을 전파한다. 

#### updateOutput()

이 함수는 새로운 회로를 구현할 때 오버라이딩받아 구현해야하는 순수 가상함수다. 진리표에 따른 출력을 이 함수에 구현한다.  

EdgeTriggered 회로의 경우 다음처럼 구현한다. 
``` c++
void IfIdRegisterCircuit::updateOutput()
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

	// update edge triggred part here!

	if (bRisingEdge)
	{
		// update edge triggred part here!
		resetDelay();
	}
}
```
마지막 clock 신호를 저장해 rising edge와 falling edge를 구분한다. 
rising edge에는 출력이 변하지 않고 상태만 변한다. delay만큼의 시간이 또 지난 후 출력이 변하게 하기 위해 resetDelay() 함수를 호출한다. 

#### afterUpdateOutput()

``` c++
void Circuit::afterUpdateOutput()
{
	// outputPin을 순회하면서 해당하는 출력이 변했다면 연결된 circuit에게 통지한다. 
	for (int i = 0; i < m_outputPinCount; i++)
	{
		OutputPin* op = GetOutputPin(i);
		assert(op != nullptr);

		bool isChanged = m_circuitOutput.IsChanged(
			op->GetOutputOffset(), 
			op->GetWireLineCount()
		);
		if (isChanged || m_delay == 0)
		{
			for (int j = 0; j < MAX_WIRE_IN_OUTPUTPIN; j++)
			{
				Wire& wire = op->GetWire(j);
				if (wire.GetTo() != nullptr)
				{
					Circuit& inputCircuit = wire.GetTo()->GetOwner();
					if (inputCircuit.IsResolvable())
					{
						inputCircuit.onInputChanged();
					}
				}
			}
		}
	}
}
```
이 함수는 udpateOutput()이 호출된 후 호출된다. 출력 데이터가 변했는지 확인해 변한 경우에만 연결된 회로의 onInputChanged() 함수를 호출해 출력을 전파한다. 

#### UpdateAll(double dt)

이 함수는 임의의 시간(dt)가 지난 후의 상태로 모든 회로를 업데이트한다. 
그런데 dt만큼의 시간을 한번에 업데이트할 수 없다. 왜냐면 시간이 지나는 중간에 상태가 업데이트될 수 있기 때문이다. 그래서 dt를 다음 상태가 변할 때까지로 쪼개서 조금씩 업데이트 시킨다. 즉, 한번에 회로들의 m_leftDelay의 최소값만큼 업데이트하며 dt를 다 resolve 해야한다. 

추가로 고려해야 하는 문제가 있다. 만약 어떤 회로의 m_leftDelay가 다 지나서 출력이 업데이트되는 동시에 해당 회로의 입력이 변경되면 어떻게 될까?  
해당 회로가 먼저 업데이트된 뒤 onInputChanged가 호출되면 정상적으로 작동하지만, onInputChanged가 먼저 호출되면 출력 업데이트-전파가 발생하지 않는 문제가 생긴다. 

이 문제를 해결하기 위해 Circuit에 m_isJustResolved 멤버 변수와 resolveConntected() 함수를 추가했다. UpdateAll에서 m_leftDelay가 다 지나 출력을 업데이트하기 전에 resolveConnected() 함수를 호출한다. resolveConnected() 함수는 출럭에 연결된 회로들 중 m_isJustResolved가 false인 회로의 출력을 업데이트-전파한 뒤 m_isJustResolved를 true로 만든다. 
