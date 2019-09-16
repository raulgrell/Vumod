#version 330 core

uniform sampler2D Texture;

in vec3 fColor;
in vec2 fUV;

void main()
{
    gl_FragColor = texture2D(Texture, vec2(-fUV.x, fUV.y)) * vec4(fColor, 1.0);
}