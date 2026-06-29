#pragma once
#include <pch.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
namespace Seed {

class Shader {
public:
    virtual ~Shader() {};

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    static Shader *Create(const std::string &vertexSrc, const std::string &fragmentSrc);
};

} // namespace Seed
