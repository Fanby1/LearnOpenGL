#include "CGBuffer.h"

CGBuffer::CGBuffer()
{
}

CGBuffer::CGBuffer(GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat, 
	GLuint vFormat, GLuint vType, unsigned int vWidth, unsigned int vHeight)
	: m_TextureUnit(vTextureUnit), m_ColorAttachment(vColorAttachment), 
	m_InternalFormat(vInternalFormat), m_Format(vFormat), m_Type(vType),
	m_Width(vWidth), m_Height(vHeight)
{
	__createGBuffer();
}

const GLuint CGBuffer::getColorAttachment() const
{
	return m_ColorAttachment;
}

CGBuffer::~CGBuffer()
{
}

void CGBuffer::__createGBuffer()
{
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, m_ColorAttachment, GL_TEXTURE_2D, m_Texture, 0);
}
