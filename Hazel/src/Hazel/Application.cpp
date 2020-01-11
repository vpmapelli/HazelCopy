#include "Application.h"

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

namespace Hazel {

        Application::Application() 
        {
        }

        Application::~Application() 
        {    
        }

        void Application::Run() 
        { 
            MouseButtonPressedEvent b(10);
            MouseButtonReleaseEvent c(10);
            MouseScrolledEvent d(20.f,20.f);
            MouseMovedEvent e(10.f,10.f);

            KeyPressedEvent f(10,10);
            KeyReleasedEvent g(10);

            WindowResizeEvent h(1280,720);
            WindowCloseEvent i;


            //HZ_TRACE(a);
            HZ_TRACE(b);
            HZ_TRACE(c);
            HZ_TRACE(d);
            HZ_TRACE(e);
            HZ_TRACE(f);
            HZ_TRACE(g);
            HZ_TRACE(h);
            HZ_TRACE(i);

            while (true){
            };   
        }
}

