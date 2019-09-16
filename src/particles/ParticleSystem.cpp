#include <util/Utils.h>
#include <platform/DisplayManager.h>
#include "ParticleSystem.h"
#include "Particle.h"
#include "ParticleTexture.h"

ParticleSystem::ParticleSystem(ParticleTexture& texture, GLfloat pps, GLfloat speed, GLfloat gravityCompliant, GLfloat lifeLength, GLfloat scale) : texture(texture)
{
	this->pps = pps;
	this->averageSpeed = speed;
	this->gravityCompliant = gravityCompliant;
	this->averageLifeLength = lifeLength;
	this->averageScale = scale;
}

void ParticleSystem::setDirection(Vec3 direction, GLfloat deviation)
{
	this->direction = direction;
	this->haveDirection = true;
	this->directionDeviation = deviation;
}

void ParticleSystem::randomizeRotation()
{
	randomRotation = true;
}

void ParticleSystem::generateParticles(Vec3 systemCenter)
{
	GLfloat delta = DisplayManager::getFrameTimeSeconds();
	GLfloat particlesToCreate = pps * delta;
	int count = floor(particlesToCreate);
	GLfloat partialParticle = particlesToCreate - (GLfloat)count;
	for (int i = 0; i < count; i++) {
		emitParticle(systemCenter);
	}
	if (Random::Float() < partialParticle) {
		emitParticle(systemCenter);
	}
}

void ParticleSystem::emitParticle(Vec3& center)
{
	//GLfloat dirX = Random::Float()) * 2.0 - 1.0;
	//GLfloat dirZ = Random::Float()) * 2.0 - 1.0;
	//Vec3 velocity(dirX, 1, dirZ);
	Vec3 velocity;
	if (haveDirection) {
		velocity = generateRandomUnitVectorWithinCone(direction, directionDeviation);
	} else {
		velocity = generateRandomUnitVector();
	}
	velocity = Vec3::Normal(velocity);

	GLfloat speed = generateValue(averageSpeed, speedError);
	velocity *= speed;
	GLfloat scale = generateValue(averageScale, scaleError);
	GLfloat lifeLength = generateValue(averageLifeLength, lifeError);

	Particle particle(&texture, center, velocity, gravityCompliant, lifeLength, generateRotation(), scale);
}

GLfloat ParticleSystem::generateValue(GLfloat average, GLfloat errorMargin)
{
	GLfloat offset = (Random::Float() - 0.5) * 2 * errorMargin;
	return average + offset;
}

GLfloat ParticleSystem::generateRotation()
{
	if (randomRotation) {
		return Random::Float() * 360;
	} else {
		return 0;
	}
}

Vec3 ParticleSystem::generateRandomUnitVectorWithinCone(Vec3& coneDirection, GLfloat angle)
{
	GLfloat cosAngle = cos(angle);
	GLfloat theta = Random::Float() * 2 * M_PI;
	GLfloat z = cosAngle + Random::Float() * (1 - cosAngle);
	GLfloat rootOneMinusZSquared = sqrt(1 - z * z);
	GLfloat x = rootOneMinusZSquared * cos(theta);
	GLfloat y = rootOneMinusZSquared * sin(theta);

	Vec4 direction(x, y, z, 1);

	if (coneDirection.x != 0 || coneDirection.y != 0 || (coneDirection.z != 1 && coneDirection.z != -1)) {
		Vec3 k(0, 0, 1);
		Vec3 rotateAxis = Vec3::Cross(coneDirection, k);
		rotateAxis = Vec3::Normal(rotateAxis);
		GLfloat value = Vec3::Dot(coneDirection, k);
		GLfloat rotateAngle = acos(value);
		Mat4 rotationMatrix = Mat4::Rotate(-rotateAngle, rotateAxis);
		direction = rotationMatrix * direction;
	} else if (coneDirection.z == -1) {
		direction.z *= -1;
	}

	return Vec3(direction.x, direction.y, direction.z);
}

Vec3 ParticleSystem::generateRandomUnitVector()
{
	GLfloat theta = Random::Float() * 2 * M_PI;
	GLfloat z = Random::Float() * 2 - 1;
	GLfloat rootOneMinusZSquared = sqrt(1 - z * z);
	GLfloat x = rootOneMinusZSquared * cos(theta);
	GLfloat y = rootOneMinusZSquared * sin(theta);
	return Vec3(x, y, z);
}
