#type vertex
#version 330

in vec2 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;
out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;

uniform vec3 cameraPosition;

uniform float waterTiling;

uniform float fogDensity;
uniform float fogGradient;

void main(void) {
	// local space -> world space
	vec4 worldPosition = transformationMatrix * vec4(position.x, 0.0, position.y, 1.0);

	// world space -> eye space
	vec4 positionRelativeToCam = viewMatrix * worldPosition;

	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;
	textureCoords = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * waterTiling;
	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * fogDensity), fogGradient));
	visibility = clamp(visibility, 0.0, 1.0);
}


//


#type fragment
#version 330

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float moveFactor;
uniform float waterReflectivity; // for fresnel effect
uniform vec3 lightColor;

uniform float waveStrength;
uniform float shineDamper;
uniform float reflectivity;

uniform float nearPlane;
uniform float farPlane;

uniform vec3 skyColor;


void main(void) {
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	float near = nearPlane;
	float far = farPlane;
	float depth = texture(depthMap, refractTexCoords).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = floorDistance - waterDistance;

	vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
	distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y  + moveFactor);

	// f makes distortion very small at small water depths
	float f = clamp(waterDepth / 20.0, 0.0, 1.0);
	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * f;

	float minTexCoord = 0.005;
	float maxTexCoord = 1.0 - minTexCoord;

	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, minTexCoord, maxTexCoord);

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, minTexCoord, maxTexCoord);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -maxTexCoord, -minTexCoord);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	// Normal map
	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	// multiply b component by a factor > 1 (e.g. 3) to make the surface more flat
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3, normalMapColor.g * 2.0 - 1.0);
	vec3 unitNormal = normalize(normal);

	// Fresnel calculation
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, unitNormal);
	refractiveFactor = pow(refractiveFactor, waterReflectivity);
	refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);

	vec3 reflectedLight = reflect(normalize(fromLightVector), unitNormal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);

	// soften specular highlights near water edge
	float g = clamp(waterDepth / 5.0, 0.0, 1.0);
	vec3 specularHighlights = lightColor * specular * reflectivity * g;

	out_Color = mix(reflectColor, refractColor, refractiveFactor);
	// make water more blue, thinmatrix had (0.0, 0.3, 0.5, 1.0), 0.2
	out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);

	out_Color = mix(vec4(skyColor, 1.0), out_Color, visibility);

	// alpha = 1 at water depth 10
	out_Color.a = clamp(waterDepth/10.0, 0.0, 1.0);
}