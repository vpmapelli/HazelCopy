#pragma once

#include "Core.h"
// #include "Events/Event.h"

#include "Window.h"

#include "Hazel/Core/LayerStack.h"

#include "Hazel/ImGui/ImGuiLayer.h"

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
        bool OnWindowResize(WindowResizeEvent& e);

#ifdef HZ_PLATFORM_LINUX
        bool OnWindowMinimize(WindowMinimizeEvent& e);
#endif

        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}