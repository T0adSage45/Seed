#include "linux_input.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"
#include "events.h"
#include "input.h"
#include "keycode.h"

namespace Seed {

InputManager *InputManager::s_Instance = new Linux_Input;

bool Linux_Input::IsKeyPressedImpl(int keycode) {

    const bool *keys = SDL_GetKeyboardState(NULL);

    if (keys[SeedKey_To_SDL(keycode)]) {
        return true;
    }
    return false;
};

bool Linux_Input::IsMousePressedImpl(int button) {
    float x, y;
    SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);
    if (buttons & SDL_BUTTON_MASK(button)) {
        return true;
    }
    return false;
};

std::pair<float, float> Linux_Input::GetMousePosImpl() {
    float x, y;
    SDL_GetMouseState(&x, &y);
    return std::pair(x, y);
};

float Linux_Input::IsMouseScroll() {

};

} // namespace Seed
