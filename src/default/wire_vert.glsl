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
}
