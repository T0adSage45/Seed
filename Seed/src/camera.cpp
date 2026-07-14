#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/matrix.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Seed {

OrthographicCam::OrthographicCam(
    float left, float top, float bottom, float right, float near, float far)
    : m_Projection_Matrix(glm::orthoLH_ZO(left, top, bottom, right, near, far)) {
    m_View_Matrix = glm::mat4(1.0f);
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

} // namespace Seed
