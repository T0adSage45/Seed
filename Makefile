#
# nix env
EXTRA_INCLUDES = \
				 -I/nix/store/45npani18v2m7sbkrzrv2xyilyghrny9-gnumake-4.4.1/include \
				 -I/nix/store/24720qn3bfvkp4c1zv1c1mvg7kvdhpzv-sdl3-3.4.2-dev/include \
				 -I/nix/store/8l5ycdza5db0jjd09lz1pcz2c1x004fj-libcxx-21.1.8-dev/include \
				 -I/nix/store/v64aggksnpk4lwng8ivglwcj6qpzvriz-lld-21.1.8-dev/include \
				 -I/nix/store/8j8vnrqibn3s0l8kpz4ah6nhpfpa2i0c-lldb-21.1.8-dev/include \
				 -I/nix/store/wznk7qyvi5v6qwd4a673v3f6zx6ngn9i-libglvnd-1.7.0-dev/include \
				 -I/nix/store/bprkgdarqhvphyn1g0vxz07zc9ay12mk-glew-2.2.0-dev/include \
				 -I/nix/store/w40cp3rmw62djczxff5ipgm046x0wgw3-glu-9.0.3-dev/include \
				 -isystem/nix/store/df6d1ggswpdwjyj36zwh93ayhvn0nnpk-glm-1.0.2/include \
				 -I/nix/store/axr5s1q5jmjsw3f2754xdm6q6rmyy12w-vulkan-headers-1.4.341.0/include \
				 -I/nix/store/2dasizwxbzyshf6hhg7p3q3ciwym999b-shaderc-2026.1-dev/include \
				 -I/nix/store/6r012bpx6frkl6kcq6a8rjdxbm9r4irl-glslang-16.2.0-dev/include \
				 -I/nix/store/alrnfsvl5zr8043sm7bs47akhmm355z1-spirv-tools-1.4.341.0-dev/include \
				 -I/nix/store/fl23713nvv8flr1lvy9ziwr9gadpw0vn-spirv-headers-1.4.341.0/include \
				 -I/nix/store/cr5lim02i1zl0ggx5bxic3r02k09mrkg-compiler-rt-libc-21.1.8-dev/include \
				 -I/nix/store/lvwga6ivl1d4lnw0zis9ajs0rqx9gp4i-gcc-15.2.0/include/c++/15.2.0 \
				 -I/nix/store/lvwga6ivl1d4lnw0zis9ajs0rqx9gp4i-gcc-15.2.0/include/c++/15.2.0/x86_64-unknown-linux-gnu \
				 -I/nix/store/hh6y3s72d21whp6q98h4dh0valxiaw69-clang-wrapper-21.1.8/resource-root/include \
				 -I/nix/store/h0ip0h6qp7kc2wm7mwjaglkxxbzmjri4-glibc-2.42-51-dev/include \
				 -I/nix/store/hh6y3s72d21whp6q98h4dh0valxiaw69-clang-wrapper-21.1.8/resource-root/include


# Compiler Configuration
CXX = clang++
CXX_STD = -std=c++20

# Build Type (debug/release, default: debug)
BUILD_TYPE ?= debug

# Directory Structure
BUILD_DIR  = build
LIB_DIR    = $(BUILD_DIR)/lib
BIN_DIR    = $(BUILD_DIR)/bin
PCH_DIR    = $(BUILD_DIR)

# Precompiled Header
PCH_SRC    = Seed/pch.h
PCH_OUTPUT = $(PCH_DIR)/pch.h.pch

# Output Targets
LIB_TARGET    = $(LIB_DIR)/libseed.so
SANDBOX_TARGET = $(BIN_DIR)/leaf

# Hybrid Nix/System SDL3 Configuration (pkg-config works in Nix shells and on system)
PKG_CFLAGS = $(shell pkg-config sdl3 gl --cflags vulkan shaderc 2>/dev/null || echo "-I/usr/include/SDL3")
PKG_LIBS   = $(shell pkg-config sdl3 gl --libs 2>/dev/null || echo "-lSDL3")

# Include Paths (Engine + ImGui + ImGui Backends + SDL3)
INCLUDES = -ISeed -ISeed/src -ISeed/src/ui -Ilib/imgui -Ilib/imgui/backends -ISeed/src/platform/Linux -ISeed/src/renderer $(PKG_CFLAGS) $(EXTRA_INCLUDES)

# Base Compiler Flags
BASE_CXXFLAGS = $(CXX_STD) -Wall -Wextra -pedantic -fPIC \
				-DGLM_FORCE_CXX17  -Wno-invalid-constexpr \
				-fno-omit-frame-pointer \
				-DSEED_PLATFORM_LINUX -DSEED_ENABLE_ASSERTS -MMD -MP

# Debug/Release Specific Flags
ifeq ($(BUILD_TYPE), debug)
    BUILD_CXXFLAGS = $(BASE_CXXFLAGS) -g
    BUILD_LDFLAGS  = -g
else ifeq ($(BUILD_TYPE), release)
    BUILD_CXXFLAGS = $(BASE_CXXFLAGS) -O3 -DNDEBUG
    BUILD_LDFLAGS  =
endif

CXXFLAGS = $(BUILD_CXXFLAGS)

# Linker Libraries
BASE_LDLIBS = -lGL -lGLEW -ldl $(PKG_LIBS)
LIB_LDLIBS = $(BASE_LDLIBS)
SANDBOX_LDLIBS = -L$(LIB_DIR) -lseed $(BASE_LDLIBS)

# LibSeed Source Files (Engine + ImGui Core + ImGui Backends + ImGui Demo)
LIB_SRCS = \
    Seed/src/app.cpp \
    Seed/src/layerstack.cpp \
    Seed/src/layers.cpp \
    Seed/src/camera.cpp \
    Seed/src/log.cpp \
    Seed/src/ui/ui.cpp \
    Seed/src/utility.cpp \
    Seed/src/platform/Linux/linux_window.cpp \
    Seed/src/platform/Linux/linux_input.cpp \
    Seed/src/renderer/opengl.cpp \
    Seed/src/renderer/render.cpp \
    Seed/src/renderer/material.cpp \
    Seed/src/renderer/buffer.cpp \
    Seed/src/renderer/shader.cpp \
    Seed/src/renderer/vulkan.cpp \
    lib/imgui/imgui.cpp \
    lib/imgui/imgui_demo.cpp \
    lib/imgui/imgui_draw.cpp \
    lib/imgui/imgui_tables.cpp \
    lib/imgui/imgui_widgets.cpp \
    lib/imgui/backends/imgui_impl_sdl3.cpp \
    lib/imgui/backends/imgui_impl_opengl3.cpp

# LibSeed Object/Dependency Files (Preserve Source Directory Structure)
LIB_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))
LIB_DEPS = $(LIB_OBJS:.o=.d)

# Sandbox Source Files
SANDBOX_SRCS = leaf/src/sandbox.cpp
SANDBOX_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SANDBOX_SRCS))
SANDBOX_DEPS = $(SANDBOX_OBJS:.o=.d)

# Phony Targets (Non-file Targets)
.PHONY: all init lib sandbox run clean rebuild debug release help perf-record flamegraph

# Default Target
all: lib sandbox

# Help
help:
	@echo "Seed Build System Targets:"
	@echo "  all (default)    Build libseed.so and leaf sandbox"
	@echo "  init              Initialize git submodules (ImGui)"
	@echo "  lib               Build libseed.so only"
	@echo "  sandbox           Build leaf sandbox only"
	@echo "  run               Build and run leaf sandbox"
	@echo "  clean             Remove build directory"
	@echo "  rebuild           Clean and rebuild all"
	@echo "  debug             Build debug mode (assertions + symbols)"
	@echo "  release           Build release mode (optimized, no assertions)"
	@echo "  perf-record       Build release + record perf data (Ctrl+C to stop)"
	@echo "  flamegraph        Generate flamegraph.svg from perf.data and open"
	@echo ""
	@echo "Override build type: make BUILD_TYPE=release"
	@echo "Profiling: make perf-record BUILDTYPE=debug  (debug build)"

# Initialize Submodules
init:
	git submodule update --init --recursive

# Precompiled Header Rule (Uses same optimization flags as source files)
$(PCH_OUTPUT): $(PCH_SRC) | $(BUILD_DIR)
	@echo "Compiling precompiled header..."
	$(CXX) $(filter-out -MMD -MP -include-pch $(PCH_OUTPUT), $(CXXFLAGS)) -x c++-header $< -o $@ $(INCLUDES)

# Build Directory Structure
$(BUILD_DIR):
	mkdir -p $(LIB_DIR) $(BIN_DIR)
	mkdir -p $(BUILD_DIR)/Seed/src/ui $(BUILD_DIR)/Seed/src/platform/Linux
	mkdir -p $(BUILD_DIR)/leaf/src
	mkdir -p $(BUILD_DIR)/lib/imgui/backends

# Pattern Rule: Compile .cpp to .o (Preserve Directory Structure)
$(BUILD_DIR)/%.o: %.cpp $(PCH_OUTPUT) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -include-pch $(PCH_OUTPUT) -c $< -o $@ $(INCLUDES)

# Build Shared Library (libseed.so)
lib: $(LIB_TARGET)

$(LIB_TARGET): $(LIB_OBJS) | $(LIB_DIR)
	@echo "Linking $(LIB_TARGET)..."
	$(CXX) -shared $(LIB_OBJS) -o $@ $(BUILD_LDFLAGS) $(LIB_LDLIBS)

# Build Sandbox Executable
sandbox: $(SANDBOX_TARGET)

$(SANDBOX_TARGET): $(SANDBOX_OBJS) $(LIB_TARGET) | $(BIN_DIR)
	@echo "Linking $(SANDBOX_TARGET)..."
	$(CXX) $(SANDBOX_OBJS) -o $@ $(BUILD_LDFLAGS) $(SANDBOX_LDLIBS)

# App
app: sandbox $(LIB_TARGET)
		$(CXX) -fno-omit-frame-pointer $(SANDBOX_OBJS) $(LIB_TARGET) -o $(BIN_DIR)/$@

# Run Sandbox
run: sandbox
	@echo "Running leaf sandbox..."
	LD_LIBRARY_PATH=$(LIB_DIR) $(SANDBOX_TARGET)

# Clean Build Artifacts
clean:
	rm -rf $(BUILD_DIR) imgui.ini perf* flamegraph*

# Rebuild All
rebuild: clean all

# Debug Build
debug:
	$(MAKE) clean
	$(MAKE) BUILD_TYPE=debug all

# Release Build
release:
	$(MAKE) clean
	$(MAKE) BUILD_TYPE=release all

# Profiling: Record perf data (run interactively, Ctrl+C to stop)
perf-record: BUILDTYPE ?= release
perf-record:
	$(MAKE) BUILD_TYPE=$(BUILDTYPE) sandbox
	@echo "Recording perf data (press Ctrl+C to stop)..."
	LD_LIBRARY_PATH=$(LIB_DIR) perf record -g -- $(SANDBOX_TARGET)

# Profiling: Generate flamegraph from existing perf.data and open in browser
flamegraph:
	perf script | stackcollapse-perf.pl | flamegraph.pl > flamegraph.svg
	xdg-open flamegraph.svg

# Include Dependency Files (Automatic Header Tracking)
-include $(LIB_DEPS) $(SANDBOX_DEPS)
