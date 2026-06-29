# Seed Engine

A C++20 game engine for "Project SAO" with modular architecture.

## Architecture

```
Seed/          → Engine library (libseed.so)
  ├── Application    → Main loop, event dispatching
  ├── LayerStack    → Manages UI/game layers
  ├── Window (interface) → Platform abstraction
  │   └── _Sdl_Window  → SDL3 + OpenGL implementation
  ├── Events         → Keyboard, window, mouse events
  └── DebugUi (Layer) → ImGui integration

leaf/          → Sandbox application
  └── sandbox.cpp   → Creates app, pushes DebugUi layer

lib/imgui/     → Dear ImGui (docking branch, submodule)
  └── backends/      → SDL3 + OpenGL3 backends included in engine
```

## Build System

### Prerequisites
- Nix (optional, for reproducible environment): `nix develop`
- Or: clang++, SDL3, pkg-config, OpenGL headers

### Quick Start
```bash
git clone --recurse-submodules <repo>
cd Seed

# Build debug (default)
make

# Build release
make BUILD_TYPE=release

# Run sandbox (shows ImGui demo window)
make run

# Clean
make clean
```

### Build Targets
| Target | Description |
|--------|-------------|
| `make` / `make all` | Build libseed.so + leaf (debug) |
| `make lib` | Build only engine library |
| `make sandbox` | Build only sandbox |
| `make run` | Build and run sandbox |
| `make release` | Optimized build (-O3, no assertions) |
| `make debug` | Debug build with symbols |
| `make clean` | Remove build/ directory |
| `make init` | Initialize git submodules |

### Build Features
- **Precompiled Headers**: `Seed/pch.h` compiled once for faster builds
- **Dependency Tracking**: Automatic `.d` files track header dependencies
- **Hybrid Nix Support**: Works in Nix shells (`nix develop`) and regular Linux
- **Shared Library**: Engine builds as `libseed.so` with ImGui bundled


## Creating a New Application

```cpp
#include <seed.h>

class MyApp : public Seed::Application {
public:
    MyApp() {
        PushOverlay(new MyGameLayer());
    }
};

Seed::Application *Seed::CreateApp() {
    return new MyApp();
}
```

## Dependencies

- **SDL3**: Windowing, input, OpenGL context
- **Dear ImGui**: Immediate-mode UI (docking branch)
- **OpenGL**: Rendering backend

## License

[Your license here]
