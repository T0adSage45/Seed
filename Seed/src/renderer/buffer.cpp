#include "buffer.h"
#include "log.h"
#include "render.h"
#include "opengl.h"
#include <cstdint>

namespace Seed {

VertexBuffer *VertexBuffer::Create(float *verts, uint32_t size) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::None:
        Seed_Trace("not suppourted right now");
        break;
    case RendererAPI::API::OpenGl:
        return new Gl_VertexBuffer(verts, size);
    case RendererAPI::API::Vulkan:
        Seed_Trace("not suppourted right now");
        break;
    }

    Seed_Trace("buffer Intialization issue");
    return nullptr;
};

IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t size) {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::None:
        Seed_Trace("not suppourted right now");
        break;
    case RendererAPI::API::OpenGl:
        return new Gl_IndexBuffer(indices, size);
    case RendererAPI::API::Vulkan:
        Seed_Trace("not suppourted right now");
        break;
    }

    Seed_Trace("buffer Intialization issue");
    return nullptr;
};

VertexArr *VertexArr::Create() {
    switch (RendererAPI::GetAPI()) {
    case RendererAPI::API::None:
        Seed_Trace("not suppourted right now");
        break;
    case RendererAPI::API::OpenGl:
        return new Gl_VertArr();
    case RendererAPI::API::Vulkan:
        Seed_Trace("not suppourted right now");
        break;
    }

    Seed_Trace("buffer Intialization issue");
    return nullptr;
};

} // namespace Seed
