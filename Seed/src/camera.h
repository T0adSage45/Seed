#pragma once
#include "pch.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace Seed {

class Camera {
public:
    virtual const glm::mat4 &GetVP_Mat() = 0;
};

class OrthographicCam : public Camera {
public:
    OrthographicCam(
        float left, float top, float bottom, float right, float near = 1.0f, float far = 100.0);

    const glm::vec3 &GetPosition() const { return m_Position; };
    void SetPosition(const glm::vec3 &pos) { m_Position = pos; };

    const glm::vec3 &GetRotation() const { return m_Rotation; };
    void SetRotation(const glm::vec3 &rot) { m_Rotation = rot; };

    const glm::mat4 &GetProjection_Mat() const { return m_Projection_Matrix; };
    const glm::mat4 &GetView_Mat() const { return m_View_Matrix; };
    const glm::mat4 &GetVP_Mat() override { return m_ViewProj_Matrix; };

    void RecalcViewMatrix();

private:
    glm::mat4 m_Projection_Matrix;
    glm::mat4 m_View_Matrix;
    glm::mat4 m_ViewProj_Matrix; // for cache-hit

    glm::vec3 m_Position;
    glm::vec3 m_Rotation; // quaternion
};

class PerspectiveCam : public Camera {
public:
    PerspectiveCam(float fovy, float aspect, float near = 0.1f, float far = 100.0f);

    const glm::vec3 &GetPosition() const { return m_Position; };
    void SetPosition(const glm::vec3 &pos) { m_Position = pos; };

    const glm::vec3 &GetRotation() const { return m_Rotation; };
    void SetRotation(const glm::vec3 &rot) { m_Rotation = rot; };

    const glm::mat4 &GetProjection_Mat() const { return m_Projection_Matrix; };
    const glm::mat4 &GetView_Mat() const { return m_View_Matrix; };
    const glm::mat4 &GetVP_Mat() override { return m_ViewProj_Matrix; };

    void RecalcViewMatrix();

private:
    glm::mat4 m_Projection_Matrix;
    glm::mat4 m_View_Matrix;
    glm::mat4 m_ViewProj_Matrix;

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
};

} // namespace Seed
