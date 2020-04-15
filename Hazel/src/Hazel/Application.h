#pragma once

#include "Core.h"
// #include "Events/Event.h"

#include "Window.h"

#include "LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

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

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;

        std::shared_ptr<Shader> m_BlueShader;
        std::shared_ptr<VertexArray> m_SquareVA;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}