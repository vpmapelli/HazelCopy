#include "WindowsInput.h"
#include "WindowsWindow.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {

    Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window , keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float,float> WindowsInput::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };      
    }

    float WindowsInput::GetMouseXImpl()
    {
        auto[x, y] = GetMousePositionImpl(); //Works on C++
        return x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        auto[x, y] = GetMousePositionImpl();
        return y;
    }

}