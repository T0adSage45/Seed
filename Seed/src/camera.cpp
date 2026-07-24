#include "camera.h"
#include "core.h"
#include "events.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/matrix.hpp"
#include "input.h"
#include "keycode.h"
#include "log.h"
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Seed {

OrthographicCam::OrthographicCam(float left, float top, float bottom, float right, float near,
                                 float far)
    : m_Projection_Matrix(glm::orthoLH_ZO(left, top, bottom, right, near, far)) {
    m_View_Matrix = (1.0f);
    m_ViewProj_Matrix = m_Projection_Matrix * m_View_Matrix;
};

void OrthographicCam::SetProjection(float left, float top, float bottom, float right, float near,
                                    float far) {
    m_Projection_Matrix = glm::orthoLH_ZO(left, top, bottom, right, near, far);
    m_ViewProj_Matrix = m_Projection_Matrix * m_View_Matrix;
};

void OrthographicCam::RecalcViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
    transform = glm::rotate(transform, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_View_Matrix = glm::inverse(transform);
    m_ViewProj_Matrix = m_Projection_Matrix * m_View_Matrix;
};

PerspectiveCam::PerspectiveCam(float fovy, float aspect, float near, float far)
    : m_Projection_Matrix(glm::perspectiveLH_ZO(fovy, aspect, near, far)) {
    m_View_Matrix = glm::mat4(1.0f);
    m_ViewProj_Matrix = m_Projection_Matrix * m_View_Matrix;
};

void PerspectiveCam::RecalcViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
    transform = glm::rotate(transform, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    m_View_Matrix = glm::inverse(transform);
    m_ViewProj_Matrix = m_Projection_Matrix * m_View_Matrix;
};

OrhtoCamCtrl::OrhtoCamCtrl(float aspect_ratio, bool rotation)
    : m_rotation(rotation),
      m_aspect_ratio(aspect_ratio),
      m_cam(-aspect_ratio * m_zoom, aspect_ratio * m_zoom, -m_zoom, m_zoom) {};

void OrhtoCamCtrl::OnEvent(Event &e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(SEED_BIND_EVENT_FN(&OrhtoCamCtrl::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizedEvent>(SEED_BIND_EVENT_FN(&OrhtoCamCtrl::OnWindowResized));
};

void OrhtoCamCtrl::OnUpdate(Timestep t) {

    m_camSpeed = 10.0 * t.GetSeconds();
    m_camPos = m_cam.GetPosition();
    m_camRot = m_cam.GetRotation();

    if (InputManager::IsKeyPressed(Seed_KeyA)) {
        Seed_Trace("left key pressed...");
        m_camPos.x += m_camSpeed;
    } else if (InputManager::IsKeyPressed(Seed_KeyD)) {
        Seed_Trace("right key pressed...");
        m_camPos.x -= m_camSpeed;
    } else if (InputManager::IsKeyPressed(Seed_KeyW)) {
        Seed_Trace("right key pressed...");
        m_camPos.y -= m_camSpeed;
    } else if (InputManager::IsKeyPressed(Seed_KeyS)) {
        Seed_Trace("right key pressed...");
        m_camPos.y += m_camSpeed;
    } else if (InputManager::IsKeyPressed(Seed_KeyQ)) {
        Seed_Trace("right key pressed...");
        m_camRot.z -= m_camSpeed;
    } else if (InputManager::IsKeyPressed(Seed_KeyE)) {
        Seed_Trace("right key pressed...");
        m_camRot.z += m_camSpeed;
    };

    m_cam.SetPosition(m_camPos);
    m_cam.SetRotation(m_camRot);
    m_cam.RecalcViewMatrix();
};

bool OrhtoCamCtrl::OnMouseScrolled(MouseScrolledEvent &e) {
    m_zoom -= e.GetScrollY() * 0.30f;
    m_cam.SetProjection(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
    Seed_Trace("MouseScrolledEvent... %f", m_zoom);
    return false;
};
bool OrhtoCamCtrl::OnWindowResized(WindowResizedEvent &e) {
    Seed_Trace("WindowResizedEvent...");
    m_aspect_ratio = (float)e.GetWidth() / (float)e.GetHeight();
    m_cam.SetProjection(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
    return false;
};

} // namespace Seed
