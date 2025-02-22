#include "Hazel/Core/Application.h"
#include "Hazel/Core/Log.h"

#include <GLFW/glfw3.h>

#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/Core/Layer.h"
#include "Hazel/Core/LayerStack.h"

#include "Hazel/Core/Input.h"

#include "Hazel/Core/Timestep.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

        Application* Application::s_Instance = nullptr;

        Application::Application() 
        {
            HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
            s_Instance = this;

            m_Window = std::unique_ptr<Window>(Window::Create());

            m_Window->SetEventCallback( BIND_EVENT_FN(OnEvent) );
            m_Window->SetVSync(false);

            m_ImGuiLayer =  new ImGuiLayer;
            PushOverlay(m_ImGuiLayer);
            
            Renderer::Init();
        }

        Application::~Application() 
        {    
        }

        void Application::PushLayer(Layer* layer)
        {
            m_LayerStack.PushLayer(layer);
        }

        void Application::PushOverlay(Layer* layer)
        {
            m_LayerStack.PushOverlay(layer);
        }

        bool Application::OnWindowClose(WindowCloseEvent& e)
        {
            m_Running = false;
            return true;
        }

        bool Application::OnWindowResize(WindowResizeEvent& e)
        {
            if (e.GetWidth() == 0 || e.GetHeight() == 0)
            {
                m_Minimized = true;
                return false;
            }


            Renderer::OnWindowResize( e.GetWidth(), e.GetHeight() );

            m_Minimized = false;
            return false;
        }

        void Application::OnEvent(Event& e)
        {

            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<WindowCloseEvent>( BIND_EVENT_FN(OnWindowClose) );
            dispatcher.Dispatch<WindowResizeEvent>( BIND_EVENT_FN(OnWindowResize) );
#ifdef HZ_PLATFORM_LINUX
            dispatcher.Dispatch<WindowMinimizeEvent>( BIND_EVENT_FN(OnWindowMinimize) );
#endif

            //HZ_TRACEHZ_CORE_TRACE("{0}", e);

            for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
            {
                (*--it)->OnEvent(e);
                if (e.Handled)
                    break;
            }

        }

#ifdef HZ_PLATFORM_LINUX
        bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
        {
            if (e.IsMinimized())
            {
                m_Minimized = true;
                return false;
            }
            m_Minimized = false;
            return false;
        }
#endif
        
        void Application::Run() 
        { 
            while (m_Running)
            {
                float time = (float)glfwGetTime(); // Platform::GetTime()
                Timestep ts = time - m_LastFrameTime;
                m_LastFrameTime = time;

                if (!m_Minimized) 
                {
                    for (Layer* layer : m_LayerStack)
                        layer->OnUpdate(ts);
                }

                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
                m_ImGuiLayer->End();
                
                m_Window->OnUpdate();
            };   
        }
}