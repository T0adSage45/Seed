#include "renderer/opengl.h"
#include "GL/glew.h"
#include "core.h"
#include "log.h"
#include "renderer/buffer.h"
#include <cstdint>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Seed {

GLcontext::GLcontext(SDL_Window *windowHandler)
    : m_seed_windowhandler(windowHandler) {};

void GLcontext::Init() {
    seed_glContext = SDL_GL_CreateContext(m_seed_windowhandler);

    SEED_CORE_ASSERT(seed_glContext, "seed glcontext not created");

    glewExperimental = GL_TRUE;
    glewInit();

    SDL_GL_MakeCurrent(m_seed_windowhandler, seed_glContext);
    // glEnable(GL_DEPTH_TEST);

    Seed_Info("%s", glGetString(GL_VENDOR));
    Seed_Info("%s", glGetString(GL_RENDERER));
    Seed_Info("%s", glGetString(GL_VERSION));
};

void GLcontext::Swapbuffer() {

};

SDL_GLContext GLcontext::GetGlContext() { return seed_glContext; };

} // namespace Seed
//

namespace Seed {

Gl_VertexBuffer::Gl_VertexBuffer(float *vertices, uint32_t size) {
    glGenBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

Gl_VertexBuffer::~Gl_VertexBuffer() { glDeleteBuffers(1, &m_RenderID); };

void Gl_VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RenderID); };

void Gl_VertexBuffer::UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };
} // namespace Seed
//

namespace Seed {

Gl_IndexBuffer::Gl_IndexBuffer(uint32_t *indices, uint32_t count)
    : m_count(count) {
    glGenBuffers(1, &m_RenderID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
};

Gl_IndexBuffer::~Gl_IndexBuffer() { glDeleteBuffers(1, &m_RenderID); };

void Gl_IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID); };

void Gl_IndexBuffer::UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };
} // namespace Seed
//

namespace Seed {

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

Gl_VertArr::Gl_VertArr() { glGenVertexArrays(1, &m_RenderID); };

Gl_VertArr::~Gl_VertArr() { glDeleteVertexArrays(1, &m_RenderID); };

void Gl_VertArr::Bind() const { glBindVertexArray(m_RenderID); };

void Gl_VertArr::UnBind() const { glBindVertexArray(0); };

void Gl_VertArr::AddVertBuffer(const std::shared_ptr<VertexBuffer> &vertbuf) {

    SEED_CORE_ASSERT(vertbuf->GetLayout().GetElems().size(), "vertex buff has no layouts");

    glBindVertexArray(m_RenderID);
    vertbuf->Bind();

    uint32_t i = 0;
    for (const auto &elem : vertbuf->GetLayout()) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, elem.GetCompCount(), ShaderDataTypeToGlBaseType(elem.Type),
                              elem.Normalized ? GL_TRUE : GL_FALSE,
                              vertbuf->GetLayout().GetStride(), (const void *)elem.Offset);
        i++;
    };

    m_vertbuff.push_back(vertbuf);
};
void Gl_VertArr::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexbuf) {

    glBindVertexArray(m_RenderID);
    indexbuf->Bind();
    m_indexbuff = indexbuf;
};

} // namespace Seed
//

namespace Seed {

Gl_Texture2D::Gl_Texture2D(const std::string &path)
    : m_Path(path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    SEED_CORE_ASSERT(data, "Texture failed to load");

    if (!data) {
        data = stbi_load("leaf/texture/checker.png", &width, &height, &channels, 0);
    };

    SEED_CORE_ASSERT(data, "Default Texture failed");

    m_Width = width;
    m_Height = height;

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else if (channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    } else if (channels == 2) {
        internalFormat = GL_RG8;
        dataFormat = GL_RG;
    } else if (channels == 1) {
        internalFormat = GL_R8;
        dataFormat = GL_RED;
    } else {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGBA;
    }

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
};

Gl_Texture2D::~Gl_Texture2D() { glDeleteTextures(1, &m_RendererID); };

void Gl_Texture2D::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
};

void Gl_Texture2D::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };

} // namespace Seed
//

namespace Seed {

void Gl_RendererAPI::Init() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
};

void Gl_RendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
void Gl_RendererAPI::SetClearColor(const glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
};
void Gl_RendererAPI::Draw(const std::shared_ptr<VertexArr> &va) {
    glDrawElements(GL_TRIANGLES, va->GetIndexBuf()->GetCount(), GL_UNSIGNED_INT, nullptr);
};

} // namespace Seed
//

namespace Seed {

Gl_Shader::Gl_Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        int i = 0;
        std::stringbuf ss;
        while (i < maxLength) {
            ss.sputc(infoLog[i]);
            i++;
        }

        Seed_Error("%s", ss.str().c_str());

        // In this simple program, we'll just leave
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        int i = 0;
        std::stringbuf ss;
        while (i < maxLength) {
            ss.sputc(infoLog[i]);
            i++;
        }

        Seed_Error("%s", ss.str().c_str());

        // In this simple program, we'll just leave
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_shaderID = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_shaderID, vertexShader);
    glAttachShader(m_shaderID, fragmentShader);

    // Link our program
    glLinkProgram(m_shaderID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_shaderID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_shaderID, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_shaderID);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Use the infoLog as you see fit.

        // In this simple program, we'll just leave
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_shaderID, vertexShader);
    glDetachShader(m_shaderID, fragmentShader);
};

Gl_Shader::~Gl_Shader() { glDeleteProgram(m_shaderID); };

void Gl_Shader::Bind() const { glUseProgram(m_shaderID); };

void Gl_Shader::UnBind() const { glUseProgram(0); };

void Gl_Shader::UploadUniform(const std::string name, const glm::mat2 mat) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
};
void Gl_Shader::UploadUniform(const std::string name, const glm::mat3 mat) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
};
void Gl_Shader::UploadUniform(const std::string name, const glm::mat4 mat) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
};

void Gl_Shader::UploadUniform(const std::string name, const glm::vec1 vec) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform1fv(loc, 1, glm::value_ptr(vec));
};
void Gl_Shader::UploadUniform(const std::string name, const glm::vec2 vec) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform2fv(loc, 1, glm::value_ptr(vec));
};
void Gl_Shader::UploadUniform(const std::string name, const glm::vec3 vec) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(vec));
};
void Gl_Shader::UploadUniform(const std::string name, const glm::vec4 vec) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(vec));
};

void Gl_Shader::UploadUniform(const std::string name, int value) {
    GLuint loc = glGetUniformLocation(m_shaderID, name.c_str());
    glUniform1i(loc, value);
};

} // namespace Seed
