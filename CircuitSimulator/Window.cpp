#include "stdafx.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Window.h"

namespace glfw_imgui
{
    bool LauchWindow(Config& config)
    {
        assert(config.OnUpdate != nullptr);

        if (config.OnStart != nullptr)
        {
            bool bRet = config.OnStart();

            if (!bRet)
            {
                return false;
            }
        }

        // init Glfw
        if (!glfwInit())
            glfwTerminate();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* pWindow = glfwCreateWindow(1280, 720, config.Title, NULL, NULL);
        if (!pWindow) {
            glfwTerminate();
        }

        int frameBufferWidth = 0;
        int frameBufferHeight = 0;
        glfwMakeContextCurrent(pWindow);
        glfwSwapInterval(1);
        glfwGetFramebufferSize(pWindow, &frameBufferWidth, &frameBufferHeight);
        if (config.IsFixed)
        {
            glfwSetWindowSizeLimits(pWindow, frameBufferWidth, frameBufferHeight, frameBufferWidth, frameBufferHeight);
        }

        // Init OpenGL
        if (gladLoadGL() == false)
        {
            assert(0);
            return false;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
         
        // Init ImGui
        ImGuiContext* pGuiContext = ImGui::CreateContext();
        ImGui::SetCurrentContext(pGuiContext);
        // ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        const char* glsl_version = "#version 120";
        ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        double lastTime = glfwGetTime();
        double nowTime = lastTime;

        // Main Loop
        while (!glfwWindowShouldClose(pWindow))
        {
            // before render
            glfwGetFramebufferSize(pWindow, &frameBufferWidth, &frameBufferHeight);
            glViewport(0, 0, frameBufferWidth, frameBufferHeight);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // before render imgui
            auto& io = ImGui::GetIO();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(io.DisplaySize);
            ImGui::Begin("Content", nullptr);

            // update & render
            nowTime = glfwGetTime();
            config.OnUpdate(nowTime - lastTime);
            lastTime = nowTime;

            // after render imgui
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(pWindow);
            glfwPollEvents();
        }

        bool bRet = true;
        if (config.OnEnd != nullptr)
        {
            if (!config.OnEnd())
            {
                bRet = false;
            }
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext(pGuiContext);
        pGuiContext = nullptr;

        glfwTerminate();

        return bRet;
    }
}
