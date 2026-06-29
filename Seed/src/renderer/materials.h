#pragma once

#include "shader.h"
#include <memory>
namespace Seed {

enum class MaterialType { FlatShader, ToonShader };

class Materials {
public:
    Materials(MaterialType mat_type);

private:
    MaterialType m_type;
    std::shared_ptr<Shader> m_Shader;
    static Materials *s_instance;
};

} // namespace Seed
