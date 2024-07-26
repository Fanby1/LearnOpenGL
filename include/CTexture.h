#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CImage.h"
class CTexture
{
private:
	GLuint m_Texture = 0;
	GLuint m_TextureUnit = 0;
public:
	CTexture(const CImage& vImage, GLuint vTextureUnit);
	void bind() const;
	GLuint getTexture() const;
};



