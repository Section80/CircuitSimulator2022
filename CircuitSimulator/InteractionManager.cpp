#include "stdafx.h"
#include <imgui_node_editor.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include "Pin.h"
#include "InteractionManager.h"

namespace ImNode = ax::NodeEditor;

void showLabel(const char* label, ImColor color = ImColor(45, 32, 32, 180));

void InteractionManager::Update(std::vector<Circuit*>* pCircuits)
{
	HandleCreate();
	HandleDelete(pCircuits);
}

void InteractionManager::HandleCreate()
{
	if (ImNode::BeginCreate())
	{
		ImNode::PinId pinId1, pinId2;
		if (ImNode::QueryNewLink(&pinId1, &pinId2))
		{
			if (pinId1 && pinId2)
			{
				Pin* p1 = Pin::GetPinById(pinId1);
				Pin* p2 = Pin::GetPinById(pinId2);
				assert(p1 != nullptr || p2 != nullptr);

				if (p1->GetKind() == p2->GetKind())
				{
					showLabel("Input/Output are not match.");
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (p1->GetWireLineCount() != p2->GetWireLineCount())
				{
					showLabel("The wire counts are not match.");
					ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else {
					InputPin* ip = nullptr;
					OutputPin* op = nullptr;

					if (p1->GetKind() == ImNode::PinKind::Input)
					{
						ip = (InputPin*)p1;
						op = (OutputPin*)p2;
					}
					else {
						ip = (InputPin*)p2;
						op = (OutputPin*)p1;
					}

					if (ip->GetFrom() == op)
					{
						showLabel("Already Connected.");
						ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
					}
					else if (!op->CanConnectNew())
					{
						showLabel("The output pin is full. ");
						ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
					}
					else {
						showLabel("+ Create Link", ImColor(32, 45, 32, 180));
						if (ImNode::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
						{
							bool bRet = op->ConnectNew(ip);
							assert(bRet);
						}
					}
				}
			}
		}
	}
	ImNode::EndCreate();
}

void InteractionManager::HandleDelete(std::vector<Circuit*>* pCircuits)
{
	if (ImNode::BeginDelete())
	{
		ImNode::LinkId linkId = 0;
		while (ImNode::QueryDeletedLink(&linkId))
		{
			Wire* pWire = Wire::GetWireByLinkId(linkId);
			assert(pWire != nullptr);

			if (ImNode::AcceptDeletedItem())
			{
				pWire->GetFrom()->Disconnect(pWire->GetTo());
			}
			
		}

		ImNode::NodeId nodeId = 0;
		while (ImNode::QueryDeletedNode(&nodeId))
		{
			Circuit* pCircuit = Circuit::GetCircuitById(nodeId);
			pCircuit->Isolate();
			delete pCircuit;
			pCircuits->erase(
				std::remove(pCircuits->begin(), pCircuits->end(), pCircuit),
				pCircuits->end()
			);
		}
	}
	ImNode::EndDelete();
}

void showLabel(const char* label, ImColor color)
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
	auto size = ImGui::CalcTextSize(label);

	auto padding = ImGui::GetStyle().FramePadding;
	auto spacing = ImGui::GetStyle().ItemSpacing;

	ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

	auto rectMin = ImGui::GetCursorScreenPos() - padding;
	auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

	auto drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
	ImGui::TextUnformatted(label);
}