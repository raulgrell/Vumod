#include "VuShader.h"

#include "glad/glad.h"

VuShader::VuShader(const char * vertex_shader_source, const char * fragment_shader_source)
{
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    CheckShader(vertex_shader, "vertex shader");

    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    CheckShader(vertex_shader, "fragment shader");

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    vao = std::make_unique<VertexArray>();

    CheckErrors();
}

void VuShader::Bind() const
{
    glUseProgram(program);
    vao->Bind();
}

void VuShader::Unbind() const
{
    vao->Unbind();
    glUseProgram(0);
}

bool CheckShader(GLuint handle, const char *desc)
{
    GLint status = 0, log_length = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);

    if ((GLboolean) status == GL_FALSE)
        fprintf(stderr, "ERROR: Shader failed to compile %s!\n", desc);

    if (log_length > 0) {
        std::vector<char> buf;
        buf.resize(log_length + 1);
        glGetShaderInfoLog(handle, log_length, nullptr, &buf[0]);
        fprintf(stderr, "%s\n", &buf[0]);
    }

    return (GLboolean) status == GL_TRUE;
}

bool CheckProgram(GLuint handle, const char *desc)
{
    GLint status = 0, log_length = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);

    if ((GLboolean) status == GL_FALSE)
        fprintf(stderr, "ERROR: Shader failed to link %s!\n", desc);

    if (log_length > 0) {
        std::vector<char> buf;
        buf.resize(log_length + 1);
        glGetProgramInfoLog(handle, log_length, nullptr, &buf[0]);
        fprintf(stderr, "%s\n", &buf[0]);
    }

    return (GLboolean) status == GL_TRUE;
}

static const char *GetGlErrorString(GLenum err)
{
    switch (err) {
        case GL_INVALID_OPERATION:return "GL_INVALID_OPERATION";
        case GL_INVALID_ENUM:return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:return "GL_INVALID_VALUE";
        case GL_OUT_OF_MEMORY:return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION:return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default:return "Unknown OpenGL error";
    }
}

int CheckErrorsInternal(const char *file, int line)
{
    GLenum err(glGetError());
    while (err != GL_NO_ERROR) {
        const char *error = GetGlErrorString(err);
        fprintf(stderr, "%s at %s: line  %d\n", error, file, line);
        err = glGetError();
    }
    return err;
}