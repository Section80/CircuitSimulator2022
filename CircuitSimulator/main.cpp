#include "stdafx.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_node_editor.h>

#include <Windows.h>    // Sleep()
#include <assert.h>

#include "PlayButton.h"
#include "InteractionManager.h"

#include "Circuit.h"
#include "AndCircuit.h"
#include "SwitchCircuit.h"
#include "BitBulbCircuit.h"
#include "NotCircuit.h"
#include "BufferCircuit.h"
#include "OrCircuit.h"
#include "Int32OutCircuit.h"

#include "RegisterCircuit.h"

namespace ImNode = ax::NodeEditor;

AndCircuit* pAnd1;
// AndCircuit* pAnd2;
// AndCircuit* pAnd3;
// AndCircuit* pAnd4;
// AndCircuit* pAnd5;

SwitchCircuit* pSwitch1;
SwitchCircuit* pSwitch2;

BitBulbCircuit* pBitBulb1;
BitBulbCircuit* pBitBulb2;

NotCircuit* pNot1;
NotCircuit* pNot2;

BufferCircuit* pBuffer1;
BufferCircuit* pBuffer2;

OrCircuit* pOr1;
OrCircuit* pOr2;

Int32OutCircuit* pInt32Out;

void SRLatch(
    OrCircuit* pOr1, OrCircuit* pOr2,
    NotCircuit* pNot1, NotCircuit* pNot2);

RegisterCircuit* pRegFile;


int main()
{
    printf("Start \n");

    // init Glfw
    if (!glfwInit())
        glfwTerminate();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(1280, 720, "OMG", NULL, NULL);
    if (!pWindow) {
        glfwTerminate();
    }

    int frameBufferWidth = 0;
    int frameBufferHeight = 0;
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(pWindow, &frameBufferWidth, &frameBufferHeight);
    // glfwSetWindowSizeLimits(pWindow, frameBufferWidth, frameBufferHeight, frameBufferWidth, frameBufferHeight);

    // Init OpenGL
    if (gladLoadGL() == false)
    {
        assert(0);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    // Init ImGui
    ImGuiContext* pGuiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(pGuiContext);

    const char* glsl_version = "#version 120";
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    ImNode::Config config;
    ImNode::EditorContext* pNodeContext = ImNode::CreateEditor(&config);
    ImNode::SetCurrentEditor(pNodeContext);

    float h = 0.0f;
    float hGap = 80.0f;
    Pin::InitVector();
    pAnd1 = new AndCircuit(5.0f);
    ImNode::SetNodePosition(pAnd1->GetNodeId(), ImVec2(0, h));
    // pAnd2 = new AndCircuit(5.0f);;
    // ImNode::SetNodePosition(pAnd2->GetNodeId(), ImVec2(300, 0));
    // pAnd3 = new AndCircuit(5.0f);;
    // ImNode::SetNodePosition(pAnd3->GetNodeId(), ImVec2(600, 0));
    // pAnd4 = new AndCircuit(5.0f);;
    // ImNode::SetNodePosition(pAnd4->GetNodeId(), ImVec2(0, 100));
    // pAnd5 = new AndCircuit(5.0f);;
    // ImNode::SetNodePosition(pAnd5->GetNodeId(), ImVec2(300, 100));
    
    pSwitch1 = new SwitchCircuit();
    ImNode::SetNodePosition(pSwitch1->GetNodeId(), ImVec2(0, h += hGap));
    pSwitch2 = new SwitchCircuit();
    ImNode::SetNodePosition(pSwitch2->GetNodeId(), ImVec2(300, h));

    pBitBulb1 = new BitBulbCircuit();
    ImNode::SetNodePosition(pBitBulb1->GetNodeId(), ImVec2(0, h += hGap));

    pBitBulb2 = new BitBulbCircuit();
    ImNode::SetNodePosition(pBitBulb2->GetNodeId(), ImVec2(300, h));

    pNot1 = new NotCircuit();
    ImNode::SetNodePosition(pNot1->GetNodeId(), ImVec2(0, h += hGap));

    pNot2 = new NotCircuit();
    ImNode::SetNodePosition(pNot2->GetNodeId(), ImVec2(300, h));

    pBuffer1 = new BufferCircuit();
    ImNode::SetNodePosition(pBuffer1->GetNodeId(), ImVec2(0, h += hGap));

    pBuffer2 = new BufferCircuit();
    ImNode::SetNodePosition(pBuffer2->GetNodeId(), ImVec2(300, h));

    pOr1 = new OrCircuit();
    ImNode::SetNodePosition(pOr1->GetNodeId(), ImVec2(0, h += hGap));

    pOr2 = new OrCircuit();
    ImNode::SetNodePosition(pOr2->GetNodeId(), ImVec2(300, h));

    SRLatch(pOr1, pOr2, pNot1, pNot2);
    pSwitch1->GetOutputPin(0)->ConnectNew(pOr1->GetInputPin(0));
    pSwitch2->GetOutputPin(0)->ConnectNew(pOr2->GetInputPin(1));
    pNot1->GetOutputPin(0)->ConnectNew(pBitBulb1->GetInputPin(0));
    pNot2->GetOutputPin(0)->ConnectNew(pBitBulb2->GetInputPin(0));

    // pSwitch1->GetOutputPin(0)->ConnectNew(pAnd1->GetInputPin(0));
    // pAnd1->GetOutputPin(0)->ConnectNew(pNot1->GetInputPin(0));
    // pNot1->GetOutputPin(0)->ConnectNew(pAnd1->GetInputPin(1));
    // pAnd1->GetOutputPin(0)->ConnectNew(pBitBulb1->GetInputPin(0));

    pRegFile = new RegisterCircuit(0, h);

    pInt32Out = new Int32OutCircuit();

    PlayButton playButton;

    double lastTime = glfwGetTime();
    double nowTime = lastTime;

    // Main Loop
    while (!glfwWindowShouldClose(pWindow))
    {
        // Update Here
        if (playButton.IsStarted())
        {
            nowTime = glfwGetTime();
            // Circuit::UpdateAll(0.05f);
            Circuit::UpdateAll(nowTime - lastTime);
            // Sleep((int)(1.0f / 60.0f) * 1000);
            lastTime = nowTime;
        }

        // Before render
        glfwGetFramebufferSize(pWindow, &frameBufferWidth, &frameBufferHeight);
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render OpenGL Here
        // ...

        // Before render ImGui
        auto& io = ImGui::GetIO();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("Content", nullptr);

        ImGui::Separator();

        // ImGui render
        playButton.Render();

        ImNode::SetCurrentEditor(pNodeContext);
        ImNode::Begin("Node Editor", ImVec2(0.0f, 0.0f));

        // ImNode render
        Circuit::RenderAll();
        Circuit::RenderAllWires();

        // ImGui Event Handler Update
        if (true)   // !playButton.IsStarted()
        {
            InteractionManager::Update();
        }

        // AfterRender ImGui
        ImNode::End();
        ImNode::SetCurrentEditor(nullptr);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    // Clean Up
    glfwTerminate();

    ImNode::DestroyEditor(pNodeContext);
    pNodeContext = nullptr;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext(pGuiContext);
    pGuiContext = nullptr;

	return 0; 
}

void SRLatch(
    OrCircuit* pOr1, OrCircuit* pOr2,
    NotCircuit* pNot1, NotCircuit* pNot2)
{
    pOr1->GetOutputPin(0)->ConnectNew(pNot1->GetInputPin(0));
    pNot1->GetOutputPin(0)->ConnectNew(pOr2->GetInputPin(0));
    pOr2->GetOutputPin(0)->ConnectNew(pNot2->GetInputPin(0));
    pNot2->GetOutputPin(0)->ConnectNew(pOr1->GetInputPin(1));
}