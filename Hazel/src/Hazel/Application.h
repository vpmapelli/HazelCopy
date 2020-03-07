#pragma once

#include "Core.h"
// #include "Events/Event.h"

#include "Window.h"

#include "LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"

namespace Hazel {

    class Event;
    class WindowCloseEvent;
    class Layer;
    class HAZEL_API Application
    {
            static Application* s_Instance;
    public:
            Application();
            virtual ~Application();

            void OnEvent(Event& e);

            void Run();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* layer);

            static inline Application& Get() { return *s_Instance; }
            inline Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}