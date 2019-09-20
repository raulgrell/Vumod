#type vertex
#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec3 vCol;
layout (location = 3) in vec2 vTexCoord;

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


//


#type fragment
#version 330 core

uniform sampler2D Texture;

in vec3 fColor;
in vec2 fUV;

void main()
{
    gl_FragColor = texture2D(Texture, vec2(-fUV.x, fUV.y)) * vec4(fColor, 1.0);
}