#include "OrthographicCameraController.h"

#include "Hazel/KeyCodes.h"
#include "Hazel/Input.h"


namespace Hazel {

    OrthographicCameraController::OrthographicCameraController(float ratio, bool rotation) 
    : m_Ratio(ratio), m_Camera(-m_ZoomLevel*ratio, m_ZoomLevel*ratio, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(Timestep ts) 
    {
        if (Input::IsKeyPressed(HZ_KEY_D))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;
        if (Input::IsKeyPressed(HZ_KEY_A))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        if (Input::IsKeyPressed(HZ_KEY_W))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        if (Input::IsKeyPressed(HZ_KEY_S))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
        
        if (m_Rotation)
        {
            if (Input::IsKeyPressed(HZ_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(HZ_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts; m_Camera.SetRotation(m_CameraRotation);        }
        
        m_Camera.SetPosition(m_CameraPosition);
        
        m_CameraMoveSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>( HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScroll) );
        dispatcher.Dispatch<WindowResizeEvent>( HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize) );
    }

    bool OrthographicCameraController::OnMouseScroll(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_ZoomLevel*m_Ratio, m_ZoomLevel*m_Ratio, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
    {
        m_Ratio = (float)e.GetWidth() / (float) e.GetHeight();
        m_Camera.SetProjection(-m_ZoomLevel*m_Ratio, m_ZoomLevel*m_Ratio, -m_ZoomLevel, m_ZoomLevel);
        return false;        
    }
}
