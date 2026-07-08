#include "texture.h"
#include "renderer/opengl.h"

namespace Seed {

Texture *Texture::Create(const std::string &path) { return new Gl_Texture2D(path); };

} // namespace Seed
