#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <imgui_node_editor.h>
#include "Window.h"
#include "SpawnCircuit.h"
#include "PlayButton.h"

namespace ImNode = ax::NodeEditor;

bool onStart();
void onUpdate(double dt);
bool onEnd();

ImNode::EditorContext* pNodeContext;
std::vector<Circuit*> pCircuits;
PlayButton playButton;

int main()
{
	glfw_imgui::Config cfg = 
	{
		1280, 720,
		false, 
		"Hello World", 
		onStart,
		onUpdate,
		onEnd
	};

	glfw_imgui::LauchWindow(cfg);

	return 0;
}

bool onStart()
{
	ImNode::Config config;
	pNodeContext = ImNode::CreateEditor(&config);
	ImNode::SetCurrentEditor(pNodeContext);

	SpawnSimple1(0, 0, &pCircuits);
	return true;
}

void onUpdate(double dt)
{
	// Update
	if (playButton.IsStarted())
	{
		Circuit::UpdateAll(dt);
		Sleep((int)(1.0f / 60.0f) * 1000);
	}

	// Render
	// ... 

	// ImGui render
	playButton.Render();

	// Before ImNode Render
	auto& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(io.DisplaySize);

	ImNode::SetCurrentEditor(pNodeContext);
	ImNode::Begin("Node Editor", ImVec2(0.0f, 0.0f));

	// ImNode Render
	Circuit::RenderAll();
	Circuit::RenderAllWires();

	// After ImNode Render
	ImNode::End();
	ImNode::SetCurrentEditor(nullptr);
}

bool onEnd()
{
	for (Circuit* pCircuit : pCircuits)
	{
		delete pCircuit;
	}

	ImNode::DestroyEditor(pNodeContext);
	pNodeContext = nullptr;

	return true;
}
