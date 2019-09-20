#type vertex
#version 330

in vec3 position;
in vec2 textureCoordinates;
in vec3 normal;

out vec2 pass_textureCoordinates;
out vec3 surfaceNormal;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition[4];

uniform float useFakeLighting;

uniform float numberOfRows;
uniform vec2 textureOffset;

uniform float fogDensity;
uniform float fogGradient;

uniform vec4 clipPlane;

void main() {
	// local space -> world space
	vec4 worldPosition = transformationMatrix * vec4(position, 1.0);

	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	// world space -> eye space
	vec4 positionRelativeToCam = viewMatrix * worldPosition;

	// eye space -> homogenous clip space
	gl_Position = projectionMatrix * positionRelativeToCam;
	pass_textureCoordinates = (textureCoordinates/numberOfRows) + textureOffset;
	
	vec3 actualNormal = normal;
	if (useFakeLighting > 0.5) {
		actualNormal = vec3(0.0, 1.0, 0.0);
	}

	surfaceNormal = (transformationMatrix * vec4(actualNormal,0.0)).xyz;
	for (int i = 0; i < 4; i++) {
		toLightVector[i] = lightPosition[i] - worldPosition.xyz;
	}
	toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * fogDensity), fogGradient));
	visibility = clamp(visibility, 0.0, 1.0);
}


//


#type fragment
#version 330 core

in vec2 pass_textureCoordinates;
in vec3 surfaceNormal;
// 4 lights - keep in sync with MAX_LIGHTS constant in StaticShader.h.
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D modelTexture;
uniform vec3 lightColor[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

// cel shading
//const float levels = 4.0; // tutorial 30 cel shading

void main() {
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for (int i = 0; i < 4; i++) {
		float distance = length(toLightVector[i]);
		float attFactor = attenuation[i].x + (attenuation[i].y * distance) + (attenuation[i].z * distance * distance);
		vec3 unitLightVector = normalize(toLightVector[i]);
		float nDot1 = dot(unitNormal, unitLightVector);
		float brightness = max(nDot1, 0.0);

		// cel shading
		//float level = floor(brightness * levels);
		//brightness = level / levels;

		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, shineDamper);

		// cel shading
		//level = floor(dampedFactor * levels);
		//dampedFactor = level / levels;

		totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attFactor;
		totalSpecular = totalSpecular + (dampedFactor * reflectivity * lightColor[i]) / attFactor;
	}
	// 0.2 = ambient light
	totalDiffuse = max(totalDiffuse, 0.2);

	vec4 textureColor = texture(modelTexture, pass_textureCoordinates);
	if (textureColor.a < 0.5) {
		discard;
	}

	out_Color = vec4(totalDiffuse,1.0) * textureColor + vec4(totalSpecular, 1.0);
	out_Color = mix(vec4(skyColor, 1.0), out_Color, visibility);
}
