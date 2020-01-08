#include "Application.h"

#include <iostream>

namespace Hazel {

        Application::Application() 
        {
        }

        Application::~Application() 
        {    
        }

        void Application::Run() 
        { 
            while (true){
                std::cout << "Hello World\n" << std::endl;
            };   
        }
}

