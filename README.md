# MIPS Simulator #

실시간으로 프로세서의 각 회로의 딜레이를 시물레이션하는 MIPS Simluator

기능: 
 - GUI를 통한 회로 편집
 - 회로 저장/불러오기
 - 회로의 Delay 표기
 - 회로 Inspector를 통한 상태 등 정보 표기
 - MIPS 명령어(and/or/add/sub/slt/lw/sw/beq/j/nop) 수행
 - 실시간/사이클 단위 명령어 수행


# 빌드 및 실행 #
개발 환경: Visual Studio 2022, Windows

다음 라이브러리들을 사용하였다. 
 - [glfw](https://github.com/glfw/glfw)
 - [imgui-node-editor](https://github.com/thedmd/imgui-node-editor)
 - [nativefiledialog](https://github.com/mlabbe/nativefiledialog)  
 
imgui-node-editor와 nativefiledialog의 경우, 빌드할 수 있는 프로젝트를 솔루션에 추가해두었다. Linux에서 빌드하는 경우 라이브러리를 직접 빌드해야 할 수 있다. 

glfw의 경우, 직접 다운받아 빌드해야 한다. 

회로 파일(.save)은 ```/CircuitSimulator2022/f```에 있다. 해당 폴더를 실행 파일이 실행되는 위치에 복사해두면 ``` ./f/cc.save ``` 파일을 불러온다. 또는 Load 버튼을 통해 cc.save 파일을 직접 로딩해도 된다.  

또한 .save 파일에는 instruction memory와 data memory에 load한 어셈블리 파일(.asm)이 절대 경로로 저장되어 있으며, .save 파일을 로드할 떄 .asm 파일이 경로에 있으면 각각을 로드한다. 

windows에서 빌드 완료된 실행 파일을 tag에서 다운로드 받을 수 있다. 

instruction 단위로 하려면 ``skip 버튼``을 누르거나 ``왼쪽 방향키``를 누르면 된다. 
one clock cycle은 6초로 sec 값을 6이 아닌 다른 값으로 설정하면 skip했을 때 instruction 단위로 실행 안 될 수도 있다.

<image src="./image/0.png">

## 메뉴 설명
 - Start/Pause: 실시간으로 회로를 실행/중지한다. 
 - sec: Skip 버튼을 눌렀을 때 건너뛸 시간 간격이다. 
 - cycle: 현재 사이클. 0부터 시작한다. 
 - Skip: 시간을 건너뛴다. Clock은 기본으로 risingEdge 직전, sec은 timePeriod(6초) 돼있으니 Skip 버튼을 누르면 한 사이클씩 건너뛸 수 있다. 오른쪽 방향키를 눌러도 된다. 
 - Save: 현재 회로를 저장한다. 
 - Load: 회로를 불러온다. 
 - Spawn: 지정한 id를 가진 회로를 생성한다. CircuitType.h 참고. 
 - Summary: 클럭과 Pipeline Register의 Control Signal에 해당하는 Wire를 렌더링하지 않는다. 

## 어셈블리 파일 불러오기
파일 확장자: .asm  

### __주의!__
__asm 파일에 .data directive가 있어도 명령어만 불러오면 데이터가 로딩되지 않는다.__  
명령어를 로드한 뒤 데이터를 별개로 로딩해야 한다.

명령어는 InstructionMemory의 Load 버튼을 눌러 로딩한다. 
<image src="./image/5.png">

데이터는 DataMemory의 Load 버튼을 눌러 로딩한다. 
<image src="./image/6.png">

# 코드 설명 #

새로운 회로를 추가하기 편하도록 클래스를 설계했다.  모든 회로들은 Circuit 클래스를 상속받아 공통된 로직은 Circuit 클래스에서 처리한다.

각 회로들은 회로 자체의 딜레이를 나타내는 m_delay와 현재 남은 delay를 나타내는 m_leftDelay 멤버를 가지고 있다. m_delay가 0인 경우, 입력 데이터가 변하자마자 해당 회로의 출력이 바로 변하고, 출력에 연결된 회로들에게 전파된다. 

m_delay가 0이 아닌 경우, 입력 데이터가 변하면 m_leftDelay의 값이 m_delay로 초기화된다. m_leftDelay의 값은 메인 루프에서 계속 호출되는 Circuit::UpdateAll(double dt) 함수에서 지난 시간만큼 감소되며, 0이 된 경우 해당 회로의 출력 업데이트-전파가 일어난다. 

## Circuit 클래스의 주요 함수

다음은 Circuit들의 공통된 로직을 처리하는 주요 함수들이다. 

### onInputChanged()

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

### updateOutput()

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

### afterUpdateOutput()

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

### UpdateAll(double dt)

이 함수는 임의의 시간(dt)가 지난 후의 상태로 모든 회로를 업데이트한다. 

그런데 dt만큼의 시간을 한번에 업데이트할 수 없다. 왜냐면 시간이 지나는 중간에 상태가 업데이트될 수 있기 때문이다. 그래서 dt를 다음 상태가 변할 때까지로 쪼개서 조금씩 업데이트 시킨다. 즉, 한번에 회로들의 m_leftDelay의 최소값만큼 업데이트하며 dt를 다 resolve 해야한다. 

추가로 고려해야 하는 문제가 있다. 만약 어떤 회로의 m_leftDelay가 다 지나서 출력이 업데이트되는 동시에 해당 회로의 입력이 변경되면 어떻게 될까?  

해당 회로가 먼저 업데이트된 뒤 onInputChanged가 호출되면 정상적으로 작동하지만, onInputChanged()가 먼저 호출되면 출력 업데이트-전파가 발생하지 않는 문제가 생긴다. 왜냐하면 onInputChanged()가 호출되면 딜레이가 초기화되기 때문이다. 

이 문제를 해결하기 위해 Circuit에 m_isJustResolved 멤버 변수와 resolveConntected() 함수를 추가했다. UpdateAll에서 m_leftDelay가 다 지나 출력을 업데이트하기 전에 resolveConnected() 함수를 호출한다. resolveConnected() 함수는 출력에 연결된 회로들 중 m_isJustResolved가 false인 회로의 출력을 업데이트-전파한 뒤 m_isJustResolved를 true로 만든다. 


# 해저드 처리 #

이 섹션은 발생할 수 있는 해저드들과 각각을 어떻게 해결했는지를 다룬다. 

## ALU Hazard

발생 예
``` asm
add $t0, $t0, $t0    # 1: MEM
add $t1, $t1, $t1    # 2: EX
add $t2, $t0, $t1    # 3: ID
```
3번 명령어는 ID 단계에서 1, 2번 명령어의 결과가 레지스터에 쓰여지기 전에 값을 읽게 된다. 이 값을 사용하면 의도와 다른 결과를 얻게 된다. 
``` asm
add $t0, $t0, $t0    # 1: WB
add $t1, $t1, $t1    # 2: MEM
add $t2, $t0, $t1    # 3: EX
```
3번 명령어가 EX 단계에 있을 때 1번 명령어의 결과는 MEM/WB Register, 2번 명령어의 결과는 EX/MEM Register에서 받아올 수 있다. 

포워딩 조건은 다음과 같다. 
``` c++
// update ForwardA
if (
	EX/MEM.RegWrite &&
	EX/MEM.WriteReg != 0 &&
	EX/MEM.WriteReg == rs
)	// EXE/MEM에서 가져와야 하는 경우
{
	ForwardA = 0b01;
}
else if (
	MEM/WB.RegWrite &&
	MEM/WB.WriteReg != 0 &&
	MEM/WB.WriteReg == rs
)	// MEM/WB에서 가져와야 하는 경우
{
	m_iForwardA = 0b10;
}
else   // 레지스터에 읽은 값 그대로 사용
{
	ForwardA = 0b00;
}

// update ForwardB
if (
	EX/MEM.RegWrite &&
	EX/MEM.WriteReg == 0 &&
	EX/MEM.WriteReg == rt
)	// EXE/MEM에서 가져와야 하는 경우
{
	ForwardB = 0b01;
}
else if (
	MEM/WB.RegWrite &&
	MEM/WB.WriteReg == rs
)	// MEM/WB에서 가져와야 하는 경우
{
	ForwardB = 0b10;
}
else
{
	ForwardB = 0b00;
}
```

회로의 모습은 다음과 같다. 
<image src="./image/1.png">

일부 와이어를 생략하였다. 

## Load Use

발생 예
``` asm
lw $t0, 16($t0)      # 1: MEM
add $t1, $t0, $t0    # 2: EX
```
EX 스테이지에서 2번 명령어를 수행할 때 $t1의 값은 메모리에서 읽는 중일 것이다. 중간에 한 사이클을 Stall하면 MEM/WB 레지스터에서 값을 포워딩받을 수 있다. 
``` asm
lw $t0, 16($t0)      # 1: WB
nop                  # 2: MEM
add $t1, $t0, $t0    # 3: EX
```
이 방법이 제대로 작동하기 위해서는 WB 스테이지에서 EX 스테이지로 레지스터의 값을 받아올 때 aluRes 값이 아닌 memToReg 컨트롤 시그널에 의해 선택된 값을 받아와야 한다. lw 명령어의 경우 aluRes 값은 DataMemory를 읽을 때 사용된 address 값이기 때문이다. 

파이프라인 레지스터의 출력을 바로 포워딩받지 않는 경우 포워딩받는 값이 정해지기까지 딜레이가 추가된다. 이 경우 추가된 딜레이는 2 to 1 Mux 하나의 딜레이 뿐이기 때문에 괜찮다고 판단했다. 

Stall을 어떻게 구현할 것인가? 
1. Stall 여부는 ID Stage에서 판단한다. 
2. Stall이 발생하면 PC에 Write를 하지 않는다. 
3. Stall이 발생하면 IF/ID Register에 Write를 하지 않는다. 
4. Control Unit의 출력 대신 0을 사용한다.  

PC와 IF/ID에 Write를 하지 않으면 다음 사이클에 똑같은 명령어가 다시 Fetch/Decoding될 것이다. 그리고 Control Unit의 출력 대신 0을 사용하는 것은 곧 현재 ID 스테이지에 있는 명령어를 nop로 바꾸는 것이다.  

Stall 여부 판단 조건은 다음과 같다. 
``` c++
stall = false;

// Check Load Use
if (
	!memWrite &&	// lw sw, Store Used1의 경우 Stall하지 않기 위해 확인한다. 
	ID/EX.memRead &&
	(
		rs == ID/EX.writeReg ||
		rt == D/EX.writeReg
	)
)
{
	stall = true;
}
```

회로의 모습은 다음과 같다. 
<image src="./image/2.png">

HazardDetectionUnit의 출력 stall이 1인 경우, 쓰기과 관련된 Control Signal은 Control Unit의 결과 대신 0을 사용한다. 또한 IF/ID.write와 PC.write은 stall 값을 not 연산해 사용한다. 

## lw sw
발생 예
``` asm
lw $t0, 16($t1)    # 1: WB
sw $t0, 16($t1)    # 2: MEM
```
2번 명령어가 MEM Stage에 Data Memory에 저장하는 값은 1번 명령어의 결과가 반영되지 않은 값이다.  

이 문제를 해결하려면 MEM Stage에서 WB Stage의 Write Data를 포워딩받으면 된다. 

포워딩 조건은 다음과 같다. 
``` c++
	if (
		MEM/WB.RegWrite && 
		EX/MEM.WriteReg == MEM/WB.WriteReg
	)
	{
		forward = true;
	}
	else
	{
		forward = false;
	}
```
2번 명령어가 sw지만 memWrite를 확인할 필요가 없다. 조건을 위와 같이 만들면 Store Used1 해저드 또한 해결할 수 있다. 대신 Load Use와 마찬가지로 MEM/WB Register의 readData 대신 memToReg로 선택된 데이터를 포워딩받아야 한다. 

회로의 모습은 다음과 같다. 
<image src="./image/3.png">

## Store Used1
발생 예
``` asm
add $t0, $t0, $t0    # 1: WB
sw $t0, 16($t1)      # 2: MEM
```
lw sw와 같은 방법으로 해결 가능하다. 

## Branch Hazard
``` asm
add $t0, $t0, $t0  # 1: EX
beq $t0, $t1, JUMP # 2: ID
```
위 경우, $t0 레지스터에 해저드가 있다. 그런데 1번 명령어의 결과는 아직 파이프라인 레지스터에 쓰여있지 않기 때문에 포워딩받을 수 없다. 

``` asm
add $t0, $t0, $t0  # 1: MEM
nop                # 2: EX
beq $t0, $t1, JUMP # 3: ID
```
하지만 위처럼 Stall을 하면 EX/MEM 레지스터에서 포워딩받을 수 있다. 

다음 경우는 어떨까? 
``` asm
add $t0, $t0, $t0  # 1: WB
nop                # 2: MEM
nop                # 3: EX
beq $t0, $t1, JUMP # 4: ID
```
Write/Read가 한 사이클 내에서 발생하기 때문에 포워딩이나 Stall이 필요하지 않다. 

Stall이 필요한 또 다른 경우는 lw 명령어와의 해저드다. 
``` asm
lw $t0, 0($t1)     # 1: EX
beq $t0, $t0, JUMP # 2: ID
``` 
lw 명령어의 Memory Read는 MEM 스테이지에 일어나고 그 결과는 WB 스테이지에 레지스터 파일에 쓰여진다. MEM 스테이지에서 포워딩받으려면 Data Memory의 Read Delay가 추가되기 때문에 사이클이 길어질 수 있다. 그러니 두번 Stall하여 WB 스테이지까지 기다린다. 
``` asm
lw $t0, 0($t1)     # 1: WB
nop                # 2: MEM
nop                # 3: EX
beq $t0, $t0, JUMP # 4: ID
``` 
ID Stage에서의 포워딩과 Stall 여부 결정은 HazardDetectionUnit에서 담당한다. 만약 Stall과 Flush가 동시에 발생하면 어떻게 될까? Flush는 무시된다. 왜냐면 Stall이 발생하면 IF/ID에 nop값이 써지지 않기 때문이다. 그렇기 때문에 HazardDetectionUnit은 Flush 여부와 상관없이 Stall 여부를 결정할 수 있다. 

Branch가 발생하면 Flush를 하게 된다. 그런데 Stall을 하게 되면 Flush가 무효된다. 
beq의 경우 그럴수 있지만 jump인데 Stall을 해버리면 안된다. 그러니 beq인지, jump인지 구분해야 한다. 

포워딩 조건은 다음과 같다. 
``` c++
// ForwardA
// Check Save Used2
if (
	EX/MEM.regWrite &&
	IF/ID.rs == EX/MEM.writeReg
	)
{
	forwardA = true;
}
else
{
	forwardA = false;
}

// ForwardB
if (
	EX/MEM.regWrite &&
	IF/ID.rt == EX/MEM.writeReg
	)
{
	forwardB = true;
}
else
{
	forwardB = false;
}
```

Stall 여부 판단 조건은 다음과 같다. 
``` c++
bool bStall = false;

// Check Load Use
if (
	!memWrite &&	// lw sw, Store Used1의 경우 Stall하지 않기 위해 확인한다. 
	ID/EX.memRead &&
	(
		rs == ID/EX.writeReg ||
		rt == D/EX.writeReg
	)
)
{
	bStall = true;
}
else if (    // Branch Hazard(alu)
	(
		branch && !jump
	) && 
	ID/EX.regWrite &&
	(
		IF/ID.rs == ID/EX.writeReg ||
		IF/ID.rt == ID/EX.writeReg
	)
)
{
	bStall = true;
}
else if (    // Branch Hazard(mem)
	(
		branch && !jump
	) && 
	(
		(
			ID/EX.memRead &&
			(
				ID/EX.writeReg == IF/ID.rs ||
				ID/EX.writeReg == IF/ID.rt
			)
		) || 
		(
			EX/MEM.memRead &&
			(
				EX/MEM.writeReg == IF/ID.rs ||
				EX/MEM.writeReg == IF/ID.rt
			)
		)
	)
)
{
	bStall = true;
}
```

관련된 회로의 모습은 LoadUse와 동일하다. 
<image src="./image/2.png">
2 To 1 Mux의 in1은 EX/MEM의 aluResult다. lw 명령어의 경우 해당 값은 address값인데 문제가 없을까? 없다. stall과 forwarding에 의해 결과적으로 해당 값 대신 적절한 값이 계산이 사용되기 때문이다. 


## Store Used2
```
add $t0, $t0, $t0    # 1: MEM
add $t1, $t1, $t1    # 2: EX
sw $t0, 16($t2)      # 3: ID
```
1번 명령어의 결과는 아직 레지스터에 쓰여지지 않은 상태지만 EX/MEM에서 레지스터 값을 포워딩받기 때문에 문제없다. 

# 테스트 #

다음 어셈블리 파일(./f/test.asm)이 정상적으로 동작하는 것을 확인하였다. 

``` asm
.data 0x10000000
.byte 1
.byte 2
.word 0x04030201	# word align

.text
.globl main
main:
	addi $t0, $0, 1   # t0: 1
	nop
	nop
	nop

# ALU Hazard
	add $t1, $0, $t0    # hazard. t1: 1
	add $t2, $t1, $t0   # hazard. t2: 2
	add $t3, $t1, $t2	# hazard. t3: 3

	sw $t2, 0($0)	# [0]: 2

# Load Use
	lw $t0, 0($0)	# t0: 2
	add $t0, $t0, $t0,  # t0: 4

# lw sw
	lw $t3, 0($0)	# t3: 2
	sw $t3, 4($0)	# [4]: 2

# store used1
	add $t0, $t0, $t0   # t0: 8
	sw $t0, 0($t0)       # [8]: 8

# JUMP
	j BRANCH1
	add $t0, $0, $0	# this should not be happen
	nop
	nop

# Branch Hazard(alu)
BRANCH1: 
	add $t0, $t0, $t0	# t0: 16
	beq $t0, $t1, BRANCH2	# branch fail 
	add $t1, $0, $t0	# t1: 16
	beq $t0, $t1, BRANCH3	# branch success
BRANCH2: 
	add $t0, $0, $0		# this should not be happen 
BRANCH3: 
	add $t0, $t0, $t0		# t0: 32

# Branch Hazard(lw)
	lw $t0, 6($t3)		# t0 : 8
	lw $t1, 6($t3)		# t1 : 8
	beq $t0, $t1, BRANCH4
	add $t0, $0, $0		# this should not be happen 
	nop 
	nop
	nop
	nop

BRANCH4:
	add $t0, $t0, $t0		# t0: 16
```
