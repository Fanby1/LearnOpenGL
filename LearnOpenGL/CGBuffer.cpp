#include "CGBuffer.h"
#include <HiveLogger.h>

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

void CGBuffer::bind() const
{
	glActiveTexture(m_TextureUnit);
	// HIVE_LOG_INFO("m_Texture: {}", m_TextureUnit - GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

CGBuffer::~CGBuffer()
{
}

#include <iostream>

// º¯Êý¼ì²é OpenGL ´íÎó
void checkOpenGLError(const char* stmt, const char* fname, int line);

#define CHECK_GL_ERROR(stmt) do { \
    stmt; \
    checkOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)

void CGBuffer::__createGBuffer()
{
	CHECK_GL_ERROR(glActiveTexture(m_TextureUnit));
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	CHECK_GL_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, m_ColorAttachment, GL_TEXTURE_2D, m_Texture, 0));
}
