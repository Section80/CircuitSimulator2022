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

#include "SpawnCircuit.h"

namespace ImNode = ax::NodeEditor;

int main()
{
    printf("======== Init ======== \n");

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
    
    // Spawn Circuits
    std::vector<Circuit*>* pCircuits = new std::vector<Circuit*>();
    // SpawnTestRegisterFile(0, 0, pCircuits);
    SpawnSimple1(0, 0, pCircuits);

    PlayButton playButton;
    float skip_sec = 0.0f;

    double lastTime = glfwGetTime();
    double nowTime = lastTime;

    // Main Loop
    while (!glfwWindowShouldClose(pWindow))
    {
        nowTime = glfwGetTime();
        if (playButton.IsStarted())
        {
            // Circuit::UpdateAll(0.05f);
            Circuit::UpdateAll(nowTime - lastTime);
            // Sleep((int)(1.0f / 60.0f) * 1000);
        }
        lastTime = nowTime;

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
        ImGui::SameLine();

        ImGui::PushItemWidth(80.0f);
        ImGui::InputFloat("sec", &skip_sec, 0, 0);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if (ImGui::Button("Skip"))
        {
            Circuit::UpdateAll(skip_sec);
        }

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
    printf("======== CleanUp ======== \n");

    std::vector<Circuit*>& circuits = *pCircuits;
    for (Circuit* pCircuit : circuits)
    {
        delete pCircuit;
    }
    delete pCircuits;

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