#ifndef PIN_H
#define PIN_H

#define MAX_WIRE_IN_OUTPUTPIN 4

#include <vector>
#include <imgui_node_editor.h>
#include "Identifiable.h"
#include "Wire.h"

namespace ImNode = ax::NodeEditor;

class Circuit;
class Pin;
class InputPin;
class OutputPin;

class Pin abstract : public Identifiable
{
public:
	Pin(Circuit& owner, const char* name, ImNode::PinKind pinkind, int wireLineCount);
	virtual ~Pin();

	virtual void Render() = 0;

	inline ImNode::PinId GetPinId() { return m_pinId; };
	inline Circuit& GetOwner() { return m_owner; }
	inline const char* GetName() { return m_name; }
	inline ImNode::PinKind GetKind() { return m_pinKind; }
	// 입/출력 bit 수
	inline int GetWireLineCount() { return m_wireLineCount; }

public:
	static Pin* GetPinById(ImNode::PinId pinId);
	static void InitVector();
private:
	static std::vector<Pin*> s_pPins;

private:
	ImNode::PinId m_pinId;
	// 해당 핀을 가지고 있는 Circuit
	Circuit& m_owner;
	const char* m_name;
	ImNode::PinKind m_pinKind;
	int m_wireLineCount;	// 입/출력 bit 수
};


class InputPin final : public Pin
{
public:
	friend class OutputPin;
public:
	InputPin(Circuit& owner, const char* name, int wireLineCount);
	virtual ~InputPin();

	void Render() override;

	inline OutputPin* GetFrom() { return m_from; }
	// InputPin의 wire들 중 해당하는 index에 있는 값을 읽는다. 
	bool ReadAt(int wireLinIndex);

private:
	OutputPin* m_from;
};


class OutputPin final : public Pin
{
public:
	OutputPin(Circuit& owner, const char* name, int dataOffset, int wireLineCount);
	~OutputPin();

	void Render();
	void RenderWire();

	inline int GetOutputOffset() { return m_dataOffset; }
	Wire& GetWire(int index);
	bool ReadAt(int index);

	bool CanConnectNew();
	bool ConnectNew(InputPin* pInputPin);
	bool Disconnect(InputPin* pInputPin);

private:
	// Owner Circuit의 Output에서의 Offset
	int m_dataOffset;

	Wire m_wires[MAX_WIRE_IN_OUTPUTPIN];
};

#endif
