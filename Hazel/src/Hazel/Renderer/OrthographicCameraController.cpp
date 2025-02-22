#include "OrthographicCameraController.h"

#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/Input.h"


namespace Hazel {

    OrthographicCameraController::OrthographicCameraController(float ratio, bool rotation) 
    : m_AspectRatio(ratio), m_Camera(-m_ZoomLevel*ratio, m_ZoomLevel*ratio, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(Timestep ts) 
    {
        if (Input::IsKeyPressed(HZ_KEY_D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        if (Input::IsKeyPressed(HZ_KEY_A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        if (Input::IsKeyPressed(HZ_KEY_W))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        if (Input::IsKeyPressed(HZ_KEY_S))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        
        if (m_Rotation)
        {
            if (Input::IsKeyPressed(HZ_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(HZ_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts; 
            
            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0)
                m_CameraRotation += 360.0f;
            
            m_Camera.SetRotation(m_CameraRotation);      
        }
        
        m_Camera.SetPosition(m_CameraPosition);
        
        m_CameraTranslationSpeed = m_ZoomLevel;
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
        m_Camera.SetProjection(-m_ZoomLevel*m_AspectRatio, m_ZoomLevel*m_AspectRatio, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float) e.GetHeight();
        float heightAdjustment = (float)e.GetHeight() /720.0f;
        m_Camera.SetProjection(-m_ZoomLevel*m_AspectRatio*heightAdjustment, m_ZoomLevel*m_AspectRatio*heightAdjustment, -m_ZoomLevel*heightAdjustment, m_ZoomLevel*heightAdjustment);
        return false;        
    }
}
