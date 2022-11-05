#ifndef WIRE_H
#define WIRE_H

#include <vector>
#include <imgui_node_editor.h>
#include "Identifiable.h"

namespace ImNode = ax::NodeEditor;

class Circuit;
class InputPin;
class OutputPin;

class Wire final : public Identifiable
{
public:
	friend class OutputPin;

public:
	Wire();
	virtual ~Wire();

	void Render();

	inline InputPin* GetTo() { return m_to; }
	inline OutputPin* GetFrom() { return m_from; }
	inline ImNode::LinkId GetLinkId() { return m_linkId; }

public:
	static Wire* GetWireByLinkId(ImNode::LinkId linkId);

private:
	static std::vector<Wire*> s_pWires;

private:
	ImNode::LinkId m_linkId;

	OutputPin* m_from;
	InputPin* m_to;
};

#endif
