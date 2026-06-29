/// AI generated why do u even think i would write something this repeatative
#pragma once

namespace Seed {

// =============================================================================
// Mouse Button Codes
// =============================================================================
#define Seed_MouseLeft 0
#define Seed_MouseRight 1
#define Seed_MouseMiddle 2
#define Seed_MouseButton4 3
#define Seed_MouseButton5 4
#define Seed_MouseButton6 5
#define Seed_MouseButton7 6
#define Seed_MouseButton8 7
#define Seed_MouseLast Seed_MouseButton8
#define Seed_MouseCount 8

// =============================================================================
// Keyboard Key Codes
// =============================================================================

// Letters (A-Z)
#define Seed_KeyA 10
#define Seed_KeyB 11
#define Seed_KeyC 12
#define Seed_KeyD 13
#define Seed_KeyE 14
#define Seed_KeyF 15
#define Seed_KeyG 16
#define Seed_KeyH 17
#define Seed_KeyI 18
#define Seed_KeyJ 19
#define Seed_KeyK 20
#define Seed_KeyL 21
#define Seed_KeyM 22
#define Seed_KeyN 23
#define Seed_KeyO 24
#define Seed_KeyP 25
#define Seed_KeyQ 26
#define Seed_KeyR 27
#define Seed_KeyS 28
#define Seed_KeyT 29
#define Seed_KeyU 30
#define Seed_KeyV 31
#define Seed_KeyW 32
#define Seed_KeyX 33
#define Seed_KeyY 34
#define Seed_KeyZ 35

// Numbers (0-9)
#define Seed_Key0 40
#define Seed_Key1 41
#define Seed_Key2 42
#define Seed_Key3 43
#define Seed_Key4 44
#define Seed_Key5 45
#define Seed_Key6 46
#define Seed_Key7 47
#define Seed_Key8 48
#define Seed_Key9 49

// Function Keys
#define Seed_KeyF1 50
#define Seed_KeyF2 51
#define Seed_KeyF3 52
#define Seed_KeyF4 53
#define Seed_KeyF5 54
#define Seed_KeyF6 55
#define Seed_KeyF7 56
#define Seed_KeyF8 57
#define Seed_KeyF9 58
#define Seed_KeyF10 59
#define Seed_KeyF11 60
#define Seed_KeyF12 61
#define Seed_KeyF13 62
#define Seed_KeyF14 63
#define Seed_KeyF15 64
#define Seed_KeyF16 65
#define Seed_KeyF17 66
#define Seed_KeyF18 67
#define Seed_KeyF19 68
#define Seed_KeyF20 69
#define Seed_KeyF21 70
#define Seed_KeyF22 71
#define Seed_KeyF23 72
#define Seed_KeyF24 73

// Modifier Keys
#define Seed_KeyLeftShift 100
#define Seed_KeyRightShift 101
#define Seed_KeyLeftCtrl 102
#define Seed_KeyRightCtrl 103
#define Seed_KeyLeftAlt 104
#define Seed_KeyRightAlt 105
#define Seed_KeyLeftSuper 106
#define Seed_KeyRightSuper 107

// Special Keys
#define Seed_KeySpace 110
#define Seed_KeyTab 111
#define Seed_KeyCapsLock 112
#define Seed_KeyNumLock 113
#define Seed_KeyScrollLock 114

// Navigation Keys
#define Seed_KeyEscape 120
#define Seed_KeyEnter 121
#define Seed_KeyBackspace 122
#define Seed_KeyDelete 123
#define Seed_KeyInsert 124
#define Seed_KeyHome 125
#define Seed_KeyEnd 126
#define Seed_KeyPageUp 127
#define Seed_KeyPageDown 128

// Arrow Keys
#define Seed_KeyUp 130
#define Seed_KeyDown 131
#define Seed_KeyLeft 132
#define Seed_KeyRight 133

// Punctuation & Symbols
#define Seed_KeyComma 140
#define Seed_KeyPeriod 141
#define Seed_KeySlash 142
#define Seed_KeySemicolon 143
#define Seed_KeyQuote 144
#define Seed_KeyLeftBracket 145
#define Seed_KeyRightBracket 146
#define Seed_KeyBackslash 147
#define Seed_KeyGraveAccent 148
#define Seed_KeyMinus 149
#define Seed_KeyEqual 150

// Numpad Keys
#define Seed_KeyNumpad0 200
#define Seed_KeyNumpad1 201
#define Seed_KeyNumpad2 202
#define Seed_KeyNumpad3 203
#define Seed_KeyNumpad4 204
#define Seed_KeyNumpad5 205
#define Seed_KeyNumpad6 206
#define Seed_KeyNumpad7 207
#define Seed_KeyNumpad8 208
#define Seed_KeyNumpad9 209
#define Seed_KeyNumpadDecimal 210
#define Seed_KeyNumpadDivide 211
#define Seed_KeyNumpadMultiply 212
#define Seed_KeyNumpadSubtract 213
#define Seed_KeyNumpadAdd 214
#define Seed_KeyNumpadEnter 215

#define Seed_Unknown 216

// =============================================================================
// Mouse Modes
// =============================================================================
enum class MouseMode {
    Normal,   // Cursor visible and unconstrained
    Captured, // Cursor hidden and locked to window (FPS-style)
    Confined, // Cursor visible but confined to window bounds
    Disabled, // All mouse input disabled
};

// =============================================================================
// Key Code Conversion Functions
// =============================================================================
int SeedKey_To_SDL(int seed_keycode);
int SDLKey_To_Seed(int sdl_keycode);
int ImGuiKey_To_Seed(int imgui_keycode);

} // namespace Seed
