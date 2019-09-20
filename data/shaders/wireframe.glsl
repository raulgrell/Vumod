#type vertex
#version 330

in vec3 vPos;
in vec3 vNorm;
in vec3 vCol;

uniform mat4 MVP;
uniform vec4 Tint;

out vec3 fColor;

void main()
{
    float dummy = (vNorm.x + vCol.x) * 0.0000000001;
    fColor = vCol + Tint.xyz;
    gl_Position = MVP * vec4(vPos.x + dummy, -vPos.y, vPos.z, 1.0);
}


//


#type fragment
#version 330

in vec3 fColor;

void main()
{
    gl_FragColor = vec4(fColor, 1.0);
}