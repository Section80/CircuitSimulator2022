#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <imgui_node_editor.h>
#include <time.h>
#include "Window.h"
#include "InteractionManager.h"
#include "SpawnCircuit.h"
#include "PlayButton.h"
#include "File.h"

namespace ImNode = ax::NodeEditor;

bool onStart();
void onUpdate(double dt);
bool onEnd();

ImNode::EditorContext* pNodeContext;
std::vector<Circuit*> pCircuits;
PlayButton playButton;
float skip_sec = 0.0f;
char saveLoadBuffer[512] = { 0 };
int spawnId = 0;

int main()
{
	glfw_imgui::Config cfg = 
	{
		1280, 720,
		false, 
		"Test Window Main", 
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

	// SpawnSimple1(0, 0, &pCircuits);
	// SpawnTestRegisterFile(0, 0, &pCircuits);

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
	ImGui::SameLine();

	ImGui::PushItemWidth(80.0f);
	ImGui::InputFloat("sec", &skip_sec, 0, 0);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if (ImGui::Button("Skip"))
	{
		Circuit::UpdateAll(skip_sec);
	}
	ImGui::SameLine();

	ImGui::PushItemWidth(160.0f);
	ImGui::InputText("fileName", saveLoadBuffer, 512);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	bool bSave = ImGui::Button("Save");
	ImGui::SameLine();
	bool bLoad = ImGui::Button("Load");
	ImGui::SameLine();
	bool bBackUp = ImGui::Button("Backup");

	ImGui::PushItemWidth(80.0f);
	ImGui::InputInt("ID", &spawnId, 0, 0);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	bool bSpawn = ImGui::Button("Spawn");
	

	// Before ImNode Render
	ImNode::SetCurrentEditor(pNodeContext);
	ImNode::Begin("Node Editor", ImVec2(0.0f, 0.0f));

	// ImNode Render
	Circuit::RenderAll();
	Circuit::RenderAllWires();

	InteractionManager::Update(&pCircuits);
	if (bSave) 	// ImNode::GetNodePosition에서 exception이 안나려면 End()하기전에 해야한다. 
	{
		SaveCircuitsToFile(saveLoadBuffer, pCircuits);
	}
	if (bLoad) 
	{
		// clear
		for (Circuit* pCircuit : pCircuits)
		{
			delete pCircuit;
		}
		pCircuits.clear();
		LoadCircuitsFromFile(saveLoadBuffer, &pCircuits);
	}
	if (bBackUp) 
	{
		char buf2[512] = { 0 };
		time_t t = time(NULL);
		sprintf_s(
			buf2, "%s.%llu.backup", 
			saveLoadBuffer, t);

		SaveCircuitsToFile(buf2, pCircuits);
	}
	if (bSpawn)
	{
		Circuit* p = SpawnCircuit((ECircuitType)spawnId, 0, -200.0f);
		pCircuits.push_back(p);
	}

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
