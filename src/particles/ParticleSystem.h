#pragma once

#include "ParticleTexture.h"

#include <math/Math.h>

class ParticleSystem {
public:
	ParticleSystem(ParticleTexture& texture, GLfloat pps, GLfloat speed, GLfloat gravityCompliant, GLfloat lifeLength, GLfloat scale);
	void setDirection(Vec3 direction, GLfloat deviation);
	void randomizeRotation();

	void setSpeedError(GLfloat error) { this->speedError = error * averageSpeed; }
	void setLifeError(GLfloat error) { this->lifeError = error * averageLifeLength; }
	void setScaleError(float error) { this->scaleError = error * averageScale; }

	void generateParticles(Vec3 systemCenter);
	void emitParticle(Vec3& center);
	GLfloat generateValue(GLfloat average, GLfloat errorMargin);
	GLfloat generateRotation();
	Vec3 generateRandomUnitVectorWithinCone(Vec3& coneDirection, GLfloat angle);
	Vec3 generateRandomUnitVector();
private:
	ParticleTexture& texture;
	GLfloat pps;
	GLfloat averageSpeed;
	GLfloat gravityCompliant;
	GLfloat averageLifeLength;
	GLfloat averageScale;

	GLfloat speedError{};
	GLfloat lifeError{};
	GLfloat scaleError = 0;
	bool randomRotation = false;
	Vec3 direction;
	bool haveDirection = false;
	GLfloat directionDeviation = 0;
};

