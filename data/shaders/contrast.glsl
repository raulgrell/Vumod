#type vertex
#version 140

in vec2 position;

out vec2 textureCoords;

void main(void){
	gl_Position = vec4(position, 0.0, 1.0);
	textureCoords = position * 0.5 + 0.5;
}

//

#type shader
#version 140

in vec2 textureCoords;

out vec4 out_Colour;

uniform sampler2D colourTexture;

const float contrast = 0.3;

void main(void){
	out_Colour = texture(colourTexture, textureCoords);
	out_Colour.rgb = (out_Colour.rgb - 0.5) * (1 + contrast) + 0.5;
}