#include "common.h"

static const char *vertex_shader_text = R"(
#version 130
uniform mat4 MVP;
uniform vec4 Tint;
in vec3 vPos;
in vec3 vNorm;
in vec3 vCol;
in vec2 vTexCoord;
out vec3 fColor;
void main()
{
    vec3 x = (vNorm + vCol + vec3(vTexCoord, 0.0)) * 0.00001;
    gl_Position = MVP * vec4(vPos + x, 1.0);
    fColor = vCol + Tint.xyz;
})";

static const char *fragment_shader_text = R"(
#version 130
in vec3 fColor;
void main()
{
    gl_FragColor = vec4(fColor, 1.0);
})";

void VuShader::Bind() {
    glUseProgram(program);
    glBindVertexArray(vao_id);
    glEnableVertexAttribArray(attr_position);
    glVertexAttribPointer(attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex), (void *)offsetof(VuVertex, position));
    glEnableVertexAttribArray(attr_normal);
    glVertexAttribPointer(attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex), (void *)offsetof(VuVertex, normal));
    glEnableVertexAttribArray(attr_color);
    glVertexAttribPointer(attr_color, 3, GL_FLOAT, GL_FALSE, sizeof(VuVertex), (void *)offsetof(VuVertex, color));
    glEnableVertexAttribArray(attr_uv);
    glVertexAttribPointer(attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(VuVertex), (void *)offsetof(VuVertex, uv));
}

void VuShader::Unbind() {
    glBindVertexArray(0);
    glUseProgram(0);
}

static void initShader(VuShader &vs)
{
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);
    CheckShader(vertex_shader, "vertex shader");

    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);
    CheckShader(vertex_shader, "fragment shader");

    vs.program = glCreateProgram();
    glAttachShader(vs.program, vertex_shader);
    glAttachShader(vs.program, fragment_shader);
    glLinkProgram(vs.program);

    CheckErrors();

    glGenVertexArrays(1, &(vs.vao_id));
    glBindVertexArray(vs.vao_id);

    vs.uniform_mvp = glGetUniformLocation(vs.program, "MVP");
    vs.uniform_tint = glGetUniformLocation(vs.program, "Tint");

    vs.attr_position = glGetAttribLocation(vs.program, "vPos");
    vs.attr_normal = glGetAttribLocation(vs.program, "vNorm");
    vs.attr_color = glGetAttribLocation(vs.program, "vCol");
    vs.attr_uv = glGetAttribLocation(vs.program, "vTexCoord");
}

static bool CheckShader(GLuint handle, const char *desc)
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

static bool CheckProgram(GLuint handle, const char *desc)
{
    GLint status = 0, log_length = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);

    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr, "ERROR: Shader failed to link %s!\n", desc);
    
    if (log_length > 0)
    {
        ImVector<char> buf;
        buf.resize((int)(log_length + 1));
        glGetProgramInfoLog(handle, log_length, nullptr, buf.begin());
        fprintf(stderr, "%s\n", buf.begin());
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

static int CheckErrorsInternal(const char *file, int line)
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