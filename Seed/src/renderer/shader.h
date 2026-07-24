#pragma once
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

    static Shader *Create(const std::string &name = "flat-shader",
                          const std::string &filepath = "Seed/src/shader/flat.glsl");
};

class ShaderLib {
public:
    void Add(const std::shared_ptr<Shader> &shader);
    std::shared_ptr<Shader> Load(const std::string &filepath);
    std::shared_ptr<Shader> Load(const std::string &name, const std::string &filepath);

    std::shared_ptr<Shader> Get(const std::string &name);

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};

} // namespace Seed
