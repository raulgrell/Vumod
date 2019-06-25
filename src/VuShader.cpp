#include "VuShader.h"

static const char *vertex_shader_text = R"(
#version 130
uniform mat4 MVP;
uniform vec4 Tint;
in vec3 vPos;
in vec3 vNorm;
in vec3 vCol;
in vec2 vTexCoord;
out vec3 fColor;
out vec2 fUV;
void main()
{
    float dummy = (vNorm.x + vCol.x) * 0.0000000001;
    fColor = vCol + Tint.xyz;
    fUV = vTexCoord;
    gl_Position = MVP * vec4(vPos.x + dummy, -vPos.y, vPos.z, 1.0);
})";

static const char *fragment_shader_text = R"(
#version 130
uniform sampler2D Texture;
in vec3 fColor;
in vec2 fUV;
void main()
{
    gl_FragColor = texture2D(Texture, vec2(-fUV.x, fUV.y)) * vec4(fColor, 1.0);
})";

VuShader::VuShader()
{
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);
    CheckShader(vertex_shader, "vertex shader");

    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);
    CheckShader(vertex_shader, "fragment shader");

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    CheckErrors();

    uniform_mvp = glGetUniformLocation(program, "MVP");
    uniform_tint = glGetUniformLocation(program, "Tint");

    attr_position = glGetAttribLocation(program, "vPos");
    attr_normal = glGetAttribLocation(program, "vNorm");
    attr_color = glGetAttribLocation(program, "vCol");
    attr_uv = glGetAttribLocation(program, "vTexCoord");

    CheckErrors();
}

void VuShader::Bind() {
    glUseProgram(program);
    glBindVertexArray(vao_id);
    glEnableVertexAttribArray(attr_position);
    glEnableVertexAttribArray(attr_normal);
    glEnableVertexAttribArray(attr_color);
    glEnableVertexAttribArray(attr_uv);
}

void VuShader::Unbind() {
    glBindVertexArray(0);
    glUseProgram(0);
}

bool CheckShader(GLuint handle, const char *desc)
{
    GLint status = 0, log_length = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);

    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr, "ERROR: Shader failed to compile %s!\n", desc);

    if (log_length > 0)
    {
        std::vector<char> buf;
        buf.resize(log_length + 1);
        glGetShaderInfoLog(handle, log_length, nullptr, &buf[0]);
        fprintf(stderr, "%s\n", &buf[0]);
    }

    return (GLboolean)status == GL_TRUE;
}

bool CheckProgram(GLuint handle, const char *desc)
{
    GLint status = 0, log_length = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);

    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr, "ERROR: Shader failed to link %s!\n", desc);

    if (log_length > 0)
    {
        std::vector<char> buf;
        buf.resize(log_length + 1);
        glGetProgramInfoLog(handle, log_length, nullptr, &buf[0]);
        fprintf(stderr, "%s\n", &buf[0]);
    }

    return (GLboolean)status == GL_TRUE;
}

static const char *GetGlErrorString(GLenum err)
{
    switch (err)
    {
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default:
            return "Unknown OpenGL error";
    }
}

int CheckErrorsInternal(const char *file, int line)
{
    GLenum err(glGetError());
    while (err != GL_NO_ERROR)
    {
        const char *error = GetGlErrorString(err);
        fprintf(stderr, "%s at %s: line  %d\n", error, file, line);
        err = glGetError();
    }
    return err;
}