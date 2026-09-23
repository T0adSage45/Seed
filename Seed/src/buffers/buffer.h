#pragma once
#include <pch.h>
#include <cstdint>
#include <vector>
#include "shader/shader.h"
#include "core.h"

namespace Seed {

struct BufferElem {
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint32_t Offset;
    bool Normalized;

    BufferElem() {};

    BufferElem(ShaderDataType type, const std::string name, bool normals = false)
        : Name(name),
          Type(type),
          Size(ShaderDataTypeSize(type)),
          Offset(0),
          Normalized(normals) {}

    uint32_t GetCompCount() const {
        switch (Type) {
        case Seed::ShaderDataType::Float:
            return 1;
        case Seed::ShaderDataType::Float2:
            return 2;
        case Seed::ShaderDataType::Float3:
            return 3;
        case Seed::ShaderDataType::Float4:
            return 4;
        case Seed::ShaderDataType::Mat3:
            return 3 * 3;
        case Seed::ShaderDataType::Mat4:
            return 4 * 4;
        case Seed::ShaderDataType::Int:
            return 1;
        case Seed::ShaderDataType::Int2:
            return 2;
        case Seed::ShaderDataType::Int3:
            return 3;
        case Seed::ShaderDataType::Int4:
            return 4;
        case Seed::ShaderDataType::Bool:
            return 1;
        case Seed::ShaderDataType::None:
            return 0;
        }

        SEED_CORE_ASSERT(false, "UnkownShader type");
        return 0;
    };
};

// BufferLayout //
class BufferLayout {
public:
    BufferLayout() {};

    BufferLayout(const std::initializer_list<BufferElem> &elem)
        : m_Elem(elem) {
        CalculateOffsetAndStride();
    };

    inline uint32_t GetStride() const { return m_stride; };
    inline const std::vector<BufferElem> &GetElems() const { return m_Elem; };

    std::vector<BufferElem>::iterator begin() { return m_Elem.begin(); }
    std::vector<BufferElem>::iterator end() { return m_Elem.end(); }
    std::vector<BufferElem>::const_iterator begin() const { return m_Elem.begin(); }
    std::vector<BufferElem>::const_iterator end() const { return m_Elem.end(); }

private:
    void CalculateOffsetAndStride() {
        uint32_t offset = 0;
        m_stride = 0;
        for (auto &elem : m_Elem) {
            elem.Offset = offset;
            offset += elem.Size;
            m_stride += elem.Size;
        };
    };

private:
    std::vector<BufferElem> m_Elem;
    uint32_t m_stride = 0;
};

// VertexBuffer //
class VertexBuffer {
public:
    virtual ~VertexBuffer() {};

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual const BufferLayout &GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout &layout) = 0;

    static VertexBuffer *Create(float *verts, uint32_t size);
    static VertexBuffer *Create(uint32_t size);
};

// IndexBuffer //
class IndexBuffer {
public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static IndexBuffer *Create(uint32_t *indices, uint32_t size);
    static IndexBuffer *Create(uint32_t size);
};

// FrameBuffers //
struct ___Seed_FrameBuff_Specs__ {
    uint32_t width, height;
    bool SwapChain_Target = false; // for renderpass hold
};

class FrameBuffers {

public:
    virtual ___Seed_FrameBuff_Specs__ &GetSpecs() = 0;

    static FrameBuffers *Create(___Seed_FrameBuff_Specs__ &fbspec);
    virtual ~FrameBuffers() {};

    virtual uint32_t GetClrAttachment() const = 0;

    virtual void Revalidate() = 0;
    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
};

// VertexArr //
class VertexArr {
public:
    virtual ~VertexArr() {}

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual void AddVertBuffer(const Seed::Ref<VertexBuffer> &vertbuf) = 0;
    virtual void SetIndexBuffer(const Seed::Ref<IndexBuffer> &indexbuf) = 0;

    virtual const std::vector<Seed::Ref<VertexBuffer>> &GetVertexBuf() const = 0;
    virtual const Seed::Ref<IndexBuffer> &GetIndexBuf() const = 0;

    static VertexArr *Create();
};

} // namespace Seed
