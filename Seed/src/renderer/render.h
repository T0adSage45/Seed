#pragma once
#include "materials.h"
#include "SDL3/SDL_video.h"
#include "camera.h"
#include "renderer/buffer.h"
#include "renderer/texture.h"
#include <glm/ext/matrix_float4x4.hpp>
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

    SDL_GLContext seed_glContext;
};

class RendererAPI {

public:
    enum class API { None = 0, OpenGl = 1, Vulkan = 3 };

public:
    virtual ~RendererAPI() = default;
    virtual void SetClearColor(const glm::vec4 color) = 0;
    virtual void Clear() = 0;

    virtual void Draw(const std::shared_ptr<VertexArr> &va) = 0;

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

    static void Draw(const std::shared_ptr<VertexArr> &va);

private:
    static std::unique_ptr<RendererAPI> s_instance;
};

class Renderer {
public:
    inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

    static void Init();
    static void OpenScene(const Scene &scene);
    static void Flush();
    static void Submit(const std::shared_ptr<VertexArr> &va, const std::shared_ptr<Materials> &mat,
                       glm::vec3 transform = glm::vec3(1.0f), glm::vec3 scale = glm::vec3(1.0f),
                       const std::shared_ptr<Texture> &texture = nullptr);

    static void CloseScene();

private:
    struct SceneData {
        glm::mat4 m_viewprojection_mat;
    };
    static SceneData *m_Scene;
};
} // namespace Seed
//
