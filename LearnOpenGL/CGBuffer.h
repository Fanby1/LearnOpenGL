#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CShader.h"
class CGBuffer
{
public:
	CGBuffer();
	CGBuffer(const std::string& vUniformName, GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat,
		GLuint vFormat, GLuint vType, unsigned int vWidth, unsigned int vHeight);
	const GLuint getColorAttachment() const;
	const GLuint getTexture() const { return m_Texture; }
	void setUniformTexture(std::shared_ptr<CShader> vShader) const;
	void bind() const;
	~CGBuffer();
private:
	std::string m_UniformName;
	GLuint m_Texture = 0;
	GLuint m_TextureUnit = 0;
	GLuint m_ColorAttachment = 0;
	GLuint m_InternalFormat = 0;
	GLuint m_Format = 0;
	GLuint m_Type = 0;
	unsigned int m_Width = 0, m_Height = 0;
	void __createGBuffer();
};

