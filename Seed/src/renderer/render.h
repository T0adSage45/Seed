#pragma once
#include "materials.h"
#include "SDL3/SDL_video.h"
#include "camera/camera.h"
#include "buffers/buffer.h"
#include "renderer/texture.h"
#include "shader/shader.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec4.hpp>

namespace Seed {

struct Scene {
    Camera &cam;
    // Enviroment env;
};

class RenderingContext {
public:
    virtual void Init() = 0;
    virtual void Swapbuffer() = 0;

    // TODO:: Abstract it
    SDL_GLContext seed_glContext;
};

class RendererAPI {

public:
    enum class API { None = 0, OpenGl = 1, Vulkan = 3 };

public:
    virtual ~RendererAPI() = default;
    virtual void SetClearColor(const glm::vec4 color) = 0;
    virtual void Clear() = 0;

    virtual void Draw(const Seed::Ref<VertexArr> &va) = 0;

    inline static API GetAPI() { return s_rendererAPI; };

    virtual void Init() = 0;

    static API s_rendererAPI;

private:
};

class RenderCmd {
public:
    RenderCmd();
    static void Init();
    static void SetClearColor(const glm::vec4 color);
    static void Clear();

    static void Draw(const Seed::Ref<VertexArr> &va);

private:
    static Seed::Scope<RendererAPI> s_instance;
};

class Renderer {
public:
    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

    static void Init();
    static void OpenScene(const Scene &scene);
    static void Flush();
    static void Submit(const Seed::Ref<VertexArr> &va, Seed::Ref<Shader> &shader, glm::vec3 transform = glm::vec3(1.0f),
                       glm::vec3 scale = glm::vec3(1.0f), const Seed::Ref<Texture> &texture = nullptr);

    static void CloseScene();

private:
    struct SceneData {
        glm::mat4 m_viewprojection_mat;
    };
    static SceneData *m_Scene;
};

class Renderer2D {
public:
    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

    static void Init();
    static void OpenScene(const Scene &scene);
    static void DrawQuad(Seed::Ref<Shader> &shader, const Seed::Ref<Texture> &texture = nullptr,
                         glm::vec2 TextCoord = {0.f, 0.f}, glm::vec2 size = {1.f, 1.f},
                         glm::vec3 transform = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

    static void CloseScene();
};

} // namespace Seed
//
