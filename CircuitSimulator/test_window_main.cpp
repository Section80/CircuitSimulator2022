#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include <imgui_node_editor.h>
#include <time.h>
#include <nfd.h>
#include "Window.h"
#include "InteractionManager.h"
#include "SpawnCircuit.h"
#include "PlayButton.h"
#include "File.h"
#include "Environment.h"

namespace ImNode = ax::NodeEditor;

bool onStart();
void onUpdate(double dt);
bool onEnd();

ImNode::EditorContext* pNodeContext;
std::vector<Circuit*> pCircuits;
PlayButton playButton;
float skip_sec = 2.9f;
int spawnId = 0;

int main(int argc, char* argv[])
{
	env::pwd.assign(argv[0]);
	size_t last = env::pwd.rfind('\\');
	env::pwd = env::pwd.substr(0, last + 1);
	printf("pwd: %s \n", env::pwd.c_str());
	

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

	std::string loadPath("/f/cc.save");
	loadPath = env::pwd + loadPath;

	LoadCircuitsFromFile(loadPath.c_str(), &pCircuits);

	return true;
}

void onUpdate(double dt)
{
	// Update
	if (playButton.IsPlaying())
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
		playButton.Pause();
		Circuit::UpdateAll(skip_sec);
	}
	ImGui::SameLine();

	bool bSave = ImGui::Button("Save");
	ImGui::SameLine();
	bool bLoad = ImGui::Button("Load");

	ImGui::SameLine();
	ImGui::PushItemWidth(80.0f);
	ImGui::InputInt("ID", &spawnId, 0, 0);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	bool bSpawn = ImGui::Button("Spawn");

	ImNode::SetCurrentEditor(pNodeContext);

	ImGui::BeginChild("Inspect", ImVec2(300, 0));
	
	ImNode::NodeId* pSelectedNodeId = new ImNode::NodeId[1];
	ImNode::GetSelectedNodes(pSelectedNodeId, 1);

	if (ImNode::GetSelectedObjectCount() == 0)
	{
		ImGui::Text("Name: None");
	}
	else 
	{
		Circuit* pC = Circuit::GetCircuitById(*pSelectedNodeId);
		if (pC == nullptr)
		{
			// wire가 선택된 경우
			// printf("[info]Object count is not 0 but pointer is null. \n");
			ImGui::Text("Name: None");
		}
		else
		{
			static char nameBuffer[64] = { 0 };
			sprintf_s(nameBuffer, "Name: %s", pC->GetName());
			ImGui::Text(nameBuffer);
			pC->RenderInspector();
		}
	}
	
	ImGui::EndChild();
	ImGui::SameLine();

	// Before ImNode Render
	ImNode::Begin("Node Editor", ImVec2(0.0f, 0.0f));

	// ImNode Render
	Circuit::RenderAll();
	Circuit::RenderAllWires();

	InteractionManager::Update(&pCircuits);
	if (bSave) 	// ImNode::GetNodePosition에서 exception이 안나려면 End()하기전에 해야한다. 
	{
		playButton.Pause();

		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog("save;", env::pwd.c_str(), &outPath);

		if (result == NFD_OKAY) {
			SaveCircuitsToFile(outPath, pCircuits);
			free(outPath);
		}
		else if (result == NFD_CANCEL) {
			puts("User pressed cancel.");
		}
		else {
			printf("Error: %s\n", NFD_GetError());
		}
	}
	if (bLoad) 
	{
		playButton.Pause();

		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("save;", env::pwd.c_str(), &outPath);

		if (result == NFD_OKAY) {
			// clear
			for (Circuit* pCircuit : pCircuits)
			{
				delete pCircuit;
			}
			pCircuits.clear();

			// puts(outPath);
			LoadCircuitsFromFile(outPath, &pCircuits);
			free(outPath);
		}
		else if (result == NFD_CANCEL) {
			puts("User pressed cancel.");
		}
		else {
			printf("Error: %s\n", NFD_GetError());
		}
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
