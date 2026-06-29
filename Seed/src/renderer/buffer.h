#pragma once
#include <pch.h>
#include "core.h"
#include <cstdint>
#include <vector>

namespace Seed {

enum class ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
    case Seed::ShaderDataType::Float:
        return 4;
    case Seed::ShaderDataType::Float2:
        return 4 * 2;
    case Seed::ShaderDataType::Float3:
        return 4 * 3;
    case Seed::ShaderDataType::Float4:
        return 4 * 4;
    case Seed::ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case Seed::ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case Seed::ShaderDataType::Int:
        return 4;
    case Seed::ShaderDataType::Int2:
        return 4 * 2;
    case Seed::ShaderDataType::Int3:
        return 4 * 3;
    case Seed::ShaderDataType::Int4:
        return 4 * 4;
    case Seed::ShaderDataType::Bool:
        return 1;
    case Seed::ShaderDataType::None:
        return 0;
    };

    SEED_CORE_ASSERT(false, "Unkown ShaderDataType");
    return 0;
};

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

class VertexBuffer {
public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual const BufferLayout &GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout &layout) = 0;

    static VertexBuffer *Create(float *verts, uint32_t size);
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static IndexBuffer *Create(uint32_t *indices, uint32_t size);
};

class VertexArr {
public:
    virtual ~VertexArr() {}

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual void AddVertBuffer(const std::shared_ptr<VertexBuffer> &vertbuf) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexbuf) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>> &GetVertexBuf() const = 0;
    virtual const std::shared_ptr<IndexBuffer> &GetIndexBuf() const = 0;

    static VertexArr *Create();
};

} // namespace Seed
