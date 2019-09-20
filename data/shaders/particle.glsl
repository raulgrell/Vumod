#type vertex
#version 330 core

in vec2 position;

in mat4 modelViewMatrix;
in vec4 texOffsets;
in float blendFactor;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

uniform mat4 projectionMatrix;

uniform float numberOfRows;

void main(void)
{
	vec2 textureCoords = position + vec2(0.5, 0.5);

	textureCoords.y = 1.0 - textureCoords.y;
	textureCoords /= numberOfRows;
	textureCoords1 = textureCoords + texOffsets.xy;
	textureCoords2 = textureCoords + texOffsets.zw;
	blend = blendFactor;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 0.0, 1.0);
}


//


#type fragment
#version 330 core

in vec2 textureCoords1;
in vec2 textureCoords2;
in float blend;

out vec4 out_color;

uniform sampler2D particleTexture;

void main(void)
{
	vec4 color1 = texture(particleTexture, textureCoords1);
	vec4 color2 = texture(particleTexture, textureCoords2);

	out_color = mix(color1, color2, blend);
}
