#include "VuWireframe.h"
#include "VuShader.h"


static const char *vertex_shader_text = R"(
#version 130
uniform mat4 MVP;
uniform vec4 Tint;
in vec3 vPos;
in vec3 vNorm;
in vec3 vCol;
out vec3 fColor;
void main()
{
    float dummy = (vNorm.x + vCol.x) * 0.0000000001;
    fColor = vCol + Tint.xyz;
    gl_Position = MVP * vec4(vPos.x + dummy, -vPos.y, vPos.z, 1.0);
})";

static const char *fragment_shader_text = R"(
#version 130
in vec3 fColor;
void main()
{
    gl_FragColor = vec4(fColor, 1.0);
})";

VuWireframe::VuWireframe()
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

    CheckErrors();
}

void VuWireframe::Bind() {
    glUseProgram(program);
    glBindVertexArray(vao_id);
    glEnableVertexAttribArray(attr_position);
    glEnableVertexAttribArray(attr_normal);
    glEnableVertexAttribArray(attr_color);
}

void VuWireframe::Unbind() {
    glBindVertexArray(0);
    glUseProgram(0);
}