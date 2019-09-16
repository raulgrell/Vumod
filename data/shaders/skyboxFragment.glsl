#version 330 core

const vec4 colour1 = vec4(0.88, 0.67, 0.219, 1.0);
const vec4 colour2 = vec4(0.2, 0.6, 0.7, 1.0);

in float pass_height;

out vec4 out_colour;

void main(void) {
	float fadeFactor = 1.0 - smoothstep(-50.0, 70.0, pass_height);
	out_colour = mix(colour2, colour1, fadeFactor);
}