#pragma once
#include "core.h"
#include <memory>
#include <pch.h>
#include <string>
#include <unordered_map>

namespace Seed {

class Shader {
public:
    virtual ~Shader() {};

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
    virtual const std::string GetName() const = 0;

    static std::shared_ptr<Shader>
    Create(const std::string &name = "fallback",
           const std::string &filepath = "Seed/src/shader/fallback.glsl");
};

class ShaderLib {
public:
    void Add(const std::shared_ptr<Shader> &shader);
    std::shared_ptr<Shader> &Load(const std::string &filepath);
    std::shared_ptr<Shader> &Load(const std::string &name, const std::string &filepath);

    std::shared_ptr<Shader> &Get(const std::string &name);

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};

enum class ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
    case Seed::ShaderDataType::Float:
        return 4;
    case Seed::ShaderDataType::Float2:
        return 4 * 2;
    case Seed::ShaderDataType::Float3:
        return 4 * 3;
    case Seed::ShaderDataType::Float4:
        return 4 * 4;
    case Seed::ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case Seed::ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case Seed::ShaderDataType::Int:
        return 4;
    case Seed::ShaderDataType::Int2:
        return 4 * 2;
    case Seed::ShaderDataType::Int3:
        return 4 * 3;
    case Seed::ShaderDataType::Int4:
        return 4 * 4;
    case Seed::ShaderDataType::Bool:
        return 1;
    case Seed::ShaderDataType::None:
        return 0;
    };

    SEED_CORE_ASSERT(false, "Unkown ShaderDataType");
    return 0;
};

} // namespace Seed
