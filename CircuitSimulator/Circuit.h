#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <vector>
#include <imgui_node_editor.h>
#include "CircuitType.h"
#include "CircuitOutput.h"
#include "Wire.h"

namespace ImNode = ax::NodeEditor;

class Pin;
class OutputPin;
class ClockBufferCircuit;

class Circuit abstract : public Identifiable
{
public:
	friend class OutputPin;
	// friend void ::ClockBufferCircuit::afterUpdateOutput(); <= 이거로 안됨
	friend class ClockBufferCircuit;

public:
	Circuit(
		const char* name, ECircuitType circuitType,
		int inputPinCount, int outputPinCount,
		bool* outputBuffer1, bool* outputBuffer2,
		int outputSize, 
		float delay
	);
	~Circuit();

	// [Pure Virtual] 
	// GUI(버튼 등) 입력에 의해 출력이 변화한 경우, 마지막에 afterUpdateOutput()
	// 를 수동으로 호출해야 한다. SwitchCircuit::render() 참고
	virtual void Render() = 0;
	virtual void RenderWire(bool bSummary = false);
	virtual void RenderInspector();

	// [Pure Virtual] 
	// 존재하지 않는 index의 경우 assert한다. 
	// @param index: 0부터 시작한다. 
	virtual InputPin* GetInputPin(int index) = 0;
	// [Pure Virtual] 
	// 존재하지 않는 index의 경우 assert한다. 
	// @param index: 0부터 시작한다. 
	virtual OutputPin* GetOutputPin(int index) = 0;

	inline const char* GetName() { return m_name; }
	inline ImNode::NodeId GetNodeId() { return m_nodeId; }
	inline int GetOutputPinCount() { return m_outputPinCount; }
	inline int GetInputPinCount() { return m_inputPinCount; }
	bool GetOutputData(int index);

	// 입력핀이 모두 연결되어있는지 확인한다. 
	bool IsResolvable();

	void SetPos(float x, float y);
	ImVec2 GetPos();
	inline ECircuitType GetType() { return m_type; }

	void Isolate();

public:
	static void UpdateAll(double dt);
	static void RenderAll();
	static void RenderAllWires(bool bSummary = false);

	static Circuit* GetCircuitById(ImNode::NodeId nodeId);

protected:
	inline void swapCircuitOutput() { m_circuitOutput.Swap(); }

	// 해당하는 outputPin에 출력을 쓴다. 
	// pData의 길이는 해당 OutputPin의 WireLineCount 이상이여야 한다. 
	void setOutputData(int outputPinindex, const bool* pData);
	void setOutputDataByValue(int outputPinindex, uint32_t value);

	// 이거 쓰는게 제일 편할 듯
	void setOutputDataByValue(OutputPin& outputPin, uint32_t value);

	// setOutputBuffer()는 pData를 OutputBufferData에 복사한다. 
	// 즉, pData를 미리 할당해두어야 한다. 
	// 그렇게 하지 않고, outputBufferData에 직접 접근하고 싶을 때 이 함수를 사용한다. 
	// [WARNING]오버런을 주의해야 한다!! 
	// [WARNING]outputPin의 WireLineCount보다 많이 쓰면 안된다!!
	bool* getOutputDataBuffer(int outputPinIndex);

	virtual void onInputChanged();

	// 각 outputPin의 출력이 변한 경우, 연결된 circuit의 onInputChagned()를 호출한다. 
	virtual void afterUpdateOutput();

	// Edge-Triggered 회로 구현에 사용한다. 
	void resetDelay();

	void renderDelay(float w = 100.0f);
	void renderIOGroup(const char* title, int index, int count);

private:
	static std::vector<Circuit*> s_pCircuits;

private:
	// [Pure Virtual]
	// 현재 입력에 기반해 outputPin을 모두 업데이트한다. 
	// InputPin::ReadAt(int wireLinIndex), Circuit::setOutputData(int outputPinindex, const bool* pData) 함수를 사용해 구현한다. 
	// 주의: afterUpdateOutput()를 호출하지 말 것! 해당 함수는 맥락에 따라 알아서 호출됨. 
	// Default: 아무것도 안함.
	virtual void updateOutput();

	// 출력단에 연결된 m_delay가 0이 아닌 circuit 중 m_leftDelay가 dt보다 작은 것이 있으면
	// 출력을 업데이트하고 전파시킨다. 
	void resolveConnected(int dt_naosec);

private:
	ImNode::NodeId m_nodeId;
	const char* m_name;
	ECircuitType m_type;
	CircuitOutput m_circuitOutput;

	int m_inputPinCount;
	int m_outputPinCount;

	int m_delay;
	int m_leftDelay;

	// Circuit::UpdateAll(float dt)에서 사용되는 값
	bool m_isJustResolved;
};

#endif
