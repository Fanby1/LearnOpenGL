#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CImage.h"
class CTexture
{
private:
	GLuint m_Texture = 0;
public:
	CTexture(const CImage& vImage);
	void bind();
	GLuint getTexture() const;
};

