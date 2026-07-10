#pragma once
#include <pch.h>
#include "render.h"

namespace Seed {

class VKcontext : public RenderingContext {
public:
    VKcontext(SDL_Window *windowHandler);
    void Init() override;
    void Swapbuffer() override;
    SDL_GLContext GetGlContext();

private:
    SDL_Window *m_seed_windowhandler;
};

} // namespace Seed
