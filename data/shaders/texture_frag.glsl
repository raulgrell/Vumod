#version 330 core

in vec3 fColor;
in vec2 fUV;

uniform sampler2D Texture;

out vec4 out_Color;

void main()
{
    out_Color = texture(Texture, vec2(-fUV.x, fUV.y)) * vec4(fColor, 1.0);
}