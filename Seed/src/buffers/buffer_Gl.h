#pragma once
#include "buffer.h"
#include "opengl.h"
#include <cstdint>

namespace Seed {

// VertexBuffer //
class Gl_VertexBuffer : public VertexBuffer {
public:
    Gl_VertexBuffer(float *vertices, uint32_t size);
    Gl_VertexBuffer(uint32_t size);
    virtual ~Gl_VertexBuffer();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual const BufferLayout &GetLayout() const override { return m_layout; };
    virtual void SetLayout(const BufferLayout &layout) override { m_layout = layout; };

private:
    uint32_t m_RenderID;
    BufferLayout m_layout;
};

// IndexBuffer //
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

// FrameBuffers //
class Gl_FrameBuffers : public FrameBuffers {
public:
    virtual ___Seed_FrameBuff_Specs__ &GetSpecs() override { return m_specs; };

    Gl_FrameBuffers(const ___Seed_FrameBuff_Specs__ &spec);
    ~Gl_FrameBuffers();

    virtual void Revalidate() override;

    virtual uint32_t GetClrAttachment() const override { return m_ColorAttachment; };
    virtual void Bind() const override;
    virtual void UnBind() const override;

private:
    ___Seed_FrameBuff_Specs__ m_specs;
    uint32_t m_RenderID = 0, m_ColorAttachment = 0, m_DepthAttachment = 0;
};

// VertexArr //
class Gl_VertArr : public VertexArr {
public:
    Gl_VertArr();
    virtual ~Gl_VertArr();
    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void AddVertBuffer(const Seed::Ref<VertexBuffer> &vertbuf) override;
    virtual void SetIndexBuffer(const Seed::Ref<IndexBuffer> &indexbuf) override;

    virtual const std::vector<Seed::Ref<VertexBuffer>> &GetVertexBuf() const override { return m_vertbuff; };
    virtual const Seed::Ref<IndexBuffer> &GetIndexBuf() const override { return m_indexbuff; };

private:
    uint32_t m_RenderID;
    std::vector<Seed::Ref<VertexBuffer>> m_vertbuff;
    Seed::Ref<IndexBuffer> m_indexbuff;
};

// Types //
static GLenum ShaderDataTypeToGlBaseType(ShaderDataType type) {
    switch (type) {
    case Seed::ShaderDataType::Float:
        return GL_FLOAT;
    case Seed::ShaderDataType::Float2:
        return GL_FLOAT;
    case Seed::ShaderDataType::Float3:
        return GL_FLOAT;
    case Seed::ShaderDataType::Float4:
        return GL_FLOAT;
    case Seed::ShaderDataType::Mat3:
        return GL_FLOAT;
    case Seed::ShaderDataType::Mat4:
        return GL_FLOAT;
    case Seed::ShaderDataType::Int:
        return GL_INT;
    case Seed::ShaderDataType::Int2:
        return GL_INT;
    case Seed::ShaderDataType::Int3:
        return GL_INT;
    case Seed::ShaderDataType::Int4:
        return GL_INT;
    case Seed::ShaderDataType::Bool:
        return GL_BOOL;
    case Seed::ShaderDataType::None:
        return GL_NONE;
    }
    SEED_CORE_ASSERT(false, "UnkownShader type");
    return 0;
};

} // namespace Seed
