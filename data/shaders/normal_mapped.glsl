#type vertex
#version 140

in vec3 position;
in vec2 textureCoordinates;
in vec3 normal;
in vec3 tangent;

out vec2 pass_textureCoordinates;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out float visibility;

uniform mat4 transformationMatrix; // objects translation, rotation and scaling in the world cooridinates
uniform mat4 projectionMatrix;     // frustum
uniform mat4 viewMatrix;           // camera
uniform vec3 lightPositionEyeSpace[4];

uniform float useFakeLighting;

uniform float numberOfRows;
uniform vec2 textureOffset;

uniform float fogDensity;
uniform float fogGradient;

uniform vec4 clipPlane;

void main() {
	vec4 worldPosition = transformationMatrix * vec4(position, 1.0);
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	mat4 modelViewMatrix = viewMatrix * transformationMatrix;
	vec4 positionRelativeToCam = modelViewMatrix * vec4(position, 1.0);
	gl_Position = projectionMatrix * positionRelativeToCam;

	pass_textureCoordinates = (textureCoordinates/numberOfRows) + textureOffset;

	vec3 surfaceNormal = (modelViewMatrix * vec4(normal, 0.0)).xyz;

	vec3 norm = normalize(surfaceNormal);
	vec3 tang = normalize((modelViewMatrix * vec4(tangent, 0.0)).xyz);
	vec3 bitang = normalize(cross(norm, tang));

	mat3 toTangentSpace = mat3(
		tang.x, bitang.x, norm.x,
		tang.y, bitang.y, norm.y,
		tang.z, bitang.z, norm.z
	);

	for (int i = 0; i < 4; i++) {
		toLightVector[i] = toTangentSpace * (lightPositionEyeSpace[i] - positionRelativeToCam.xyz);
	}
	toCameraVector = toTangentSpace * (-positionRelativeToCam.xyz);

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * fogDensity), fogGradient));
	visibility = clamp(visibility, 0.0, 1.0);
}


//


#type fragment;
#version 140

in vec2 pass_textureCoordinates;
in vec3 surfaceNormal;
// The shaders use 4 light sources. If this is changed, change also the
// MAX_LIGHTS constant in StaticShader.h.
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D modelTexture;
uniform sampler2D normalMap;
uniform vec3 lightColor[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

// cel shading
//const float levels = 4.0; // tutorial 30 cel shading

void main() {
	vec4 normalMapValue = 2.0 * texture(normalMap, pass_textureCoordinates) - 1.0;

	vec3 unitNormal = normalize(normalMapValue.rgb);
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
