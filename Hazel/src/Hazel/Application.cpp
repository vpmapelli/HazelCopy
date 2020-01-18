#include "Application.h"
#include "Hazel/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/Layer.h"
#include "Hazel/LayerStack.h"

#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

        Application* Application::s_Instance = nullptr;

        Application::Application() 
        {
            HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
            s_Instance = this;

            m_Window = std::unique_ptr<Window>(Window::Create());

            m_Window->SetEventCallback( BIND_EVENT_FN(OnEvent) );
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

        void Application::OnEvent(Event& e)
        {

            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<WindowCloseEvent>( BIND_EVENT_FN(OnWindowClose) );

            HZ_CORE_TRACE("{0}", e);

            for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
            {
                (*--it)->OnEvent(e);
                if (e.Handled)
                    break;
            }

        }
        
        void Application::Run() 
        { 
            while (m_Running){
                glClearColor(1, 0, 1, 1);
                glClear(GL_COLOR_BUFFER_BIT);

                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate();

                auto[x, y] = Input::GetMousePosition();
                HZ_CORE_TRACE("{0}, {1}", x, y);

                float xPos = Input::GetMouseX();
                HZ_CORE_TRACE("{0}", xPos);
    
                float yPos = Input::GetMouseY();
                HZ_CORE_TRACE("{0}", yPos);

                bool keyPress = Input::IsKeyPressed(GLFW_KEY_S);
                HZ_CORE_TRACE("{0}", keyPress);
                
                bool mousePress = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1);
                HZ_CORE_TRACE("{0}", mousePress);
                
                m_Window->OnUpdate();
            };   
        }
}

