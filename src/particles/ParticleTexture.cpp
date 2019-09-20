#include "ParticleTexture.h"

ParticleTexture::ParticleTexture(GLuint textureID, int numberOfRows, bool additive)
{
	this->textureID = textureID;
	this->numberOfRows = numberOfRows;
	this->additive = additive;
}

GLuint ParticleTexture::GetTextureId()
{
	return this->textureID;
}

int ParticleTexture::GetNumberOfRows()
{
	return this->numberOfRows;
}

bool ParticleTexture::IsAdditive()
{
	return this->additive;
}
