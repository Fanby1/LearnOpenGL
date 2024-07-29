#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class CGBuffer
{
public:
	CGBuffer();
	CGBuffer(GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat, 
		GLuint vFormat, GLuint vType, unsigned int vWidth, unsigned int vHeight);
	const GLuint getColorAttachment() const;
	void bind() const;
	~CGBuffer();
private:
	GLuint m_Texture = 0;
	GLuint m_TextureUnit = 0;
	GLuint m_ColorAttachment = 0;
	GLuint m_InternalFormat = 0;
	GLuint m_Format = 0;
	GLuint m_Type = 0;
	unsigned int m_Width = 0, m_Height = 0;
	void __createGBuffer();
};

