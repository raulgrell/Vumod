#version 330 core

in vec3 vPos;
in vec3 vNorm;
in vec3 vCol;
in vec2 vTexCoord;

uniform mat4 MVP;
uniform vec4 Tint;

out vec3 fColor;
out vec2 fUV;

void main()
{
    float dummy = (vNorm.x + vCol.x) * 0.0000000001;
    fColor = vCol + Tint.xyz;
    fUV = vTexCoord;
    gl_Position = MVP * vec4(vPos.x + dummy, -vPos.y, vPos.z, 1.0);
}