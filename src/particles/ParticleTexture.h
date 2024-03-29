#pragma once

#include <glad/glad.h>

class ParticleTexture
{
public:
    ParticleTexture(GLuint textureID, int numberOfRows, bool additive);
    GLuint GetTextureId();
    int GetNumberOfRows();
    bool IsAdditive();
private:
    GLuint textureID;
    int numberOfRows = 1;
    bool additive;
};
