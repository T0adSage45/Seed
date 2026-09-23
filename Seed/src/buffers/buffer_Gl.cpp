#include "buffer_Gl.h"
#include "core.h"
#include "opengl.h"

namespace Seed {

// VertexBuffer //
Gl_VertexBuffer::Gl_VertexBuffer(float *vertices, uint32_t size) {
    glGenBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

Gl_VertexBuffer::Gl_VertexBuffer(uint32_t size) {
    glGenBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
};

Gl_VertexBuffer::~Gl_VertexBuffer() { glDeleteBuffers(1, &m_RenderID); };
void Gl_VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RenderID); };
void Gl_VertexBuffer::UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };

// IndexBuffer //
Gl_IndexBuffer::Gl_IndexBuffer(uint32_t *indices, uint32_t count)
    : m_count(count) {
    SEED_ASSERT(indices != nullptr, "Error in setting indices...");
    glGenBuffers(1, &m_RenderID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
};

Gl_IndexBuffer::~Gl_IndexBuffer() { glDeleteBuffers(1, &m_RenderID); };
void Gl_IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID); };
void Gl_IndexBuffer::UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

// VertArr //
Gl_VertArr::Gl_VertArr() { glGenVertexArrays(1, &m_RenderID); };
Gl_VertArr::~Gl_VertArr() { glDeleteVertexArrays(1, &m_RenderID); };
void Gl_VertArr::Bind() const { glBindVertexArray(m_RenderID); };
void Gl_VertArr::UnBind() const { glBindVertexArray(0); };

void Gl_VertArr::AddVertBuffer(const Seed::Ref<VertexBuffer> &vertbuf) {
    SEED_ASSERT(vertbuf->GetLayout().GetElems().size(), "vertex buff has no layouts");
    glBindVertexArray(m_RenderID);
    vertbuf->Bind();
    uint32_t i = 0;
    for (const auto &elem : vertbuf->GetLayout()) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, elem.GetCompCount(), ShaderDataTypeToGlBaseType(elem.Type),
                              elem.Normalized ? GL_TRUE : GL_FALSE, vertbuf->GetLayout().GetStride(),
                              (const void *)elem.Offset); // look at me .... (fix warn)
        i++;
    };
    m_vertbuff.push_back(vertbuf);
};

void Gl_VertArr::SetIndexBuffer(const Seed::Ref<IndexBuffer> &indexbuf) {
    SEED_ASSERT(indexbuf->GetCount() != 0, "indexs are not deined");
    glBindVertexArray(m_RenderID);
    indexbuf->Bind();
    m_indexbuff = indexbuf;
};

Gl_FrameBuffers::Gl_FrameBuffers(const ___Seed_FrameBuff_Specs__ &spec)
    : m_specs(spec) {
    Revalidate();
};

Gl_FrameBuffers::~Gl_FrameBuffers() {
    glDeleteFramebuffers(1, &m_RenderID);
    glDeleteTextures(1, &m_ColorAttachment);
    glDeleteTextures(1, &m_DepthAttachment);
};

void Gl_FrameBuffers::Revalidate() {

    if (!m_RenderID) {
        glDeleteFramebuffers(1, &m_RenderID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    glGenFramebuffers(1, &m_RenderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_specs.width, m_specs.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    glGenRenderbuffers(1, &m_DepthAttachment);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

    SEED_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "framebuffer is incomplete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void Gl_FrameBuffers::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID); };
void Gl_FrameBuffers::UnBind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glDeleteFramebuffers(1, &m_RenderID);
};

}; // namespace Seed
