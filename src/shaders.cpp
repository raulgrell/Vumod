#include "common.h"

static const char *vertex_shader_text = R"(
#version 130
uniform mat4 MVP;
attribute vec3 vCol;
attribute vec2 vPos;
varying vec3 color;
void main()
{
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
})";

static const char *fragment_shader_text = R"(
#version 130
varying vec3 color;
void main()
{
    gl_FragColor = vec4(color, 1.0);
})";

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] = {
    {-0.6f, -0.4f, 1.f, 0.f, 0.f},
    {0.6f, -0.4f, 0.f, 1.f, 0.f},
    {0.f, 0.6f, 0.f, 0.f, 1.f}};

static void initShader(VuShader &vs)
{
    vs.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs.vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vs.vertex_shader);

    vs.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs.fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(vs.fragment_shader);

    vs.program = glCreateProgram();
    glAttachShader(vs.program, vs.vertex_shader);
    glAttachShader(vs.program, vs.fragment_shader);
    glLinkProgram(vs.program);

    glGenBuffers(1, &vs.vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vs.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vs.mvp_location = glGetUniformLocation(vs.program, "MVP");
    vs.vpos_location = glGetAttribLocation(vs.program, "vPos");
    vs.vcol_location = glGetAttribLocation(vs.program, "vCol");
    glEnableVertexAttribArray(vs.vpos_location);
    glVertexAttribPointer(vs.vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void *)0);
    glEnableVertexAttribArray(vs.vcol_location);
    glVertexAttribPointer(vs.vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void *)(sizeof(float) * 2));
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
        ImVector<char> buf;
        buf.resize((int)(log_length + 1));
        glGetShaderInfoLog(handle, log_length, NULL, (GLchar *)buf.begin());
        fprintf(stderr, "%s\n", buf.begin());
    }
    return (GLboolean)status == GL_TRUE;
}

// If you get an error please report on GitHub. You may try different GL context version or GLSL version.
static bool CheckProgram(GLuint handle, const char *desc)
{
    GLint status = 0, log_length = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &status);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
    if ((GLboolean)status == GL_FALSE)
        fprintf(stderr, "ERROR: ImGui_GL_CreateDeviceObjects: failed to link %s!\n", desc);
    if (log_length > 0)
    {
        ImVector<char> buf;
        buf.resize((int)(log_length + 1));
        glGetProgramInfoLog(handle, log_length, NULL, (GLchar *)buf.begin());
        fprintf(stderr, "%s\n", buf.begin());
    }
    return (GLboolean)status == GL_TRUE;
}