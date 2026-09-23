#include "renderer/opengl.h"
#include "GL/glew.h"
#include "core.h"
#include "log.h"
#include "utility/utility.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

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

    // Seed_Info("w: %d, h: %d ", width, height);

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
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
    bool wireframe = false;

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
};

void Gl_RendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
void Gl_RendererAPI::SetClearColor(const glm::vec4 color) { glClearColor(color.r, color.g, color.b, color.a); };
void Gl_RendererAPI::Draw(const Seed::Ref<VertexArr> &va) { glDrawElements(GL_TRIANGLES, va->GetIndexBuf()->GetCount(), GL_UNSIGNED_INT, nullptr); };

} // namespace Seed
//

namespace Seed {
GLenum Gl_Shader::ShaderTypeFromString(std::string &type) {
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    SEED_CORE_ASSERT(false, "Unkown shader type");
    return 0;
};

Gl_Shader::Gl_Shader(const std::string &name, const std::string &filepath) {

    std::string result;
    Read_File(filepath, result);

    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char *typeToken = "#type";
        size_t typeTokenLen = strlen(typeToken);
        size_t pos = result.find(typeToken, 0);

        while (pos != std::string::npos) {
            size_t eol = result.find_first_of("\r\n", pos);
            SEED_CORE_ASSERT(eol != std::string::npos, "Syntax Error in shader: %s", filepath.c_str());
            size_t begin = pos + typeTokenLen + 1;
            std::string type = result.substr(begin, eol - begin);
            SEED_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader specification...");

            size_t nextlinePos = result.find_first_not_of("\r\n", eol);
            pos = result.find(typeToken, nextlinePos);
            shaderSources[ShaderTypeFromString(type)] =
                result.substr(nextlinePos, pos - (nextlinePos == std::string::npos ? result.size() - 1 : nextlinePos));
        }

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar *source = shaderSources[GL_VERTEX_SHADER].c_str();
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
        source = shaderSources[GL_FRAGMENT_SHADER].c_str();
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

        // Always detach shaders after a successful link.
        glDetachShader(m_shaderID, vertexShader);
        glDetachShader(m_shaderID, fragmentShader);
    }

    m_Name = name;

    // Seed_Fatal("just a manual process stopper");
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
