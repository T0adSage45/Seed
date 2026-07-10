#include "vulkan.h"
#include "SDL3/SDL_video.h"

namespace Seed {

VKcontext::VKcontext(SDL_Window *windowHandler)
    : m_seed_windowhandler(windowHandler) {};
void VKcontext::Init() {};
void VKcontext::Swapbuffer() {};
SDL_GLContext VKcontext::GetGlContext() { return seed_glContext; };

} // namespace Seed
