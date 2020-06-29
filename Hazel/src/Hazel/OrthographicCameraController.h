#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float ratio, bool rotation = false);
        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);
    private:
        bool OnMouseScroll(MouseScrolledEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        float m_Ratio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;

        float m_CameraMoveSpeed = 5.0f;
        float m_CameraRotationSpeed = 180.0f;

        float m_Rotation;
    };
}