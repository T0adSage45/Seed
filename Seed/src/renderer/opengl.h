#pragma once
#include "pch.h"
#include "GL/glew.h"
#include "SDL3/SDL_video.h"
#include "buffer.h"
#include "render.h"
#include "texture.h"
#include <cstdint>
#include <glm/ext/vector_float1.hpp>
#include <glm/ext/vector_float3.hpp>

namespace Seed {
class GLcontext : public RenderingContext {
public:
    GLcontext(SDL_Window *windowHandler);
    void Init() override;
    void Swapbuffer() override;
    SDL_GLContext GetGlContext();

private:
    SDL_Window *m_seed_windowhandler;
};

class Gl_VertexBuffer : public VertexBuffer {
public:
    Gl_VertexBuffer(float *vertices, uint32_t size);
    virtual ~Gl_VertexBuffer();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual const BufferLayout &GetLayout() const override { return m_layout; };
    virtual void SetLayout(const BufferLayout &layout) override { m_layout = layout; };

private:
    uint32_t m_RenderID;
    BufferLayout m_layout;
};

class Gl_IndexBuffer : public IndexBuffer {
public:
    Gl_IndexBuffer(uint32_t *indices, uint32_t count);
    virtual ~Gl_IndexBuffer();
    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual uint32_t GetCount() const override { return m_count; };

private:
    uint32_t m_RenderID;
    uint32_t m_count;
};

class Gl_VertArr : public VertexArr {
public:
    Gl_VertArr();
    virtual ~Gl_VertArr();
    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void AddVertBuffer(const std::shared_ptr<VertexBuffer> &vertbuf) override;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexbuf) override;

    virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuf() const override {
        return m_vertbuff;
    };
    virtual const std::shared_ptr<IndexBuffer> &GetIndexBuf() const override {
        return m_indexbuff;
    };

private:
    uint32_t m_RenderID;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertbuff;
    std::shared_ptr<IndexBuffer> m_indexbuff;
};

} // namespace Seed

namespace Seed {

class Gl_Texture2D : public Texture {
public:
    Gl_Texture2D(const std::string &path);
    virtual ~Gl_Texture2D();

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;

    virtual uint32_t GetWidth() const override { return m_Width; };
    virtual uint32_t GetHeight() const override { return m_Height; };
    virtual const std::string &GetPath() const override { return m_Path; };

private:
    uint32_t m_RendererID;
    uint32_t m_Width, m_Height;
    std::string m_Path;
};

} // namespace Seed

namespace Seed {

class Gl_RendererAPI : public RendererAPI {
public:
    ~Gl_RendererAPI() override {};
    void SetClearColor(const glm::vec4 color) override;
    void Clear() override;
    void Draw(const std::shared_ptr<VertexArr> &va) override;
    void Init() override;
};

} // namespace Seed

namespace Seed {

class Gl_Shader : public Shader {
public:
    static GLenum ShaderTypeFromString(std::string &type);

    Gl_Shader(const std::string &name, const std::string &filepath);
    virtual ~Gl_Shader();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual const std::string GetName() const override { return m_Name; };

    void UploadUniform(const std::string name, const glm::mat2 mat);
    void UploadUniform(const std::string name, const glm::mat3 mat);
    void UploadUniform(const std::string name, const glm::mat4 mat);
    void UploadUniform(const std::string name, const glm::vec1 vec);
    void UploadUniform(const std::string name, int value);
    void UploadUniform(const std::string name, const glm::vec2 vec);
    void UploadUniform(const std::string name, const glm::vec3 vec);
    void UploadUniform(const std::string name, const glm::vec4 vec);

private:
    uint32_t m_shaderID;
    std::string m_Name;
};
} // namespace Seed
