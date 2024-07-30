#include "CFramebuffer.h"
#include <vector>
#include <HiveLogger.h>

#include <iostream>

// º¯Êý¼ì²é OpenGL ´íÎó
void checkOpenGLError(const char* stmt, const char* fname, int line);

#define CHECK_GL_ERROR(stmt) do { \
    stmt; \
    checkOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)


CFramebuffer::CFramebuffer(unsigned int vWidth, unsigned int vHeight)
{
	m_Width = vWidth;
	m_Height = vHeight;
	__createFramebuffer();
}

void CFramebuffer::bind() const
{
	CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
}

void CFramebuffer::createAndAddGBuffer(const std::string& vUniformName, GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat,
	GLuint vFormat, GLuint vType)
{
	bind();
	std::shared_ptr<CGBuffer> gBuffer = std::make_shared<CGBuffer>(vUniformName, vTextureUnit, 
		vColorAttachment, vInternalFormat, vFormat, vType, m_Width, m_Height);
	m_GBuffers.insert(gBuffer);
}

void CFramebuffer::createAndAddDepthBuffer(const std::string& vUniformName, GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat,
	GLuint vFormat, GLuint vType)
{
	bind();
	m_DepthBuffer = std::make_shared<CGBuffer>(vUniformName, vTextureUnit,
		vColorAttachment, vInternalFormat, vFormat, vType, m_Width, m_Height);
}

const std::set<std::shared_ptr<CGBuffer>> CFramebuffer::getGBuffers() const
{
	return m_GBuffers;
}

void CFramebuffer::render() const
{
	bind();
	std::vector<GLuint> Attachments;
	for (auto& gBuffer : m_GBuffers)
	{
		Attachments.push_back(gBuffer->getColorAttachment());
		HIVE_LOG_INFO("Color Attachment: {}", gBuffer->getColorAttachment());
	}
	std::sort<std::vector<GLuint>::iterator>(Attachments.begin(), Attachments.end());
	CHECK_GL_ERROR(glDrawBuffers(Attachments.size(), Attachments.data()));

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBuffer->getTexture(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		HIVE_LOG_ERROR("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFramebuffer::__createFramebuffer()
{
	CHECK_GL_ERROR(glGenFramebuffers(1, &m_Framebuffer));
}
