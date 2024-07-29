#include "CFramebuffer.h"
#include <vector>
#include <HiveLogger.h>

#include <iostream>

// 函数检查 OpenGL 错误
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

void CFramebuffer::createAndAddGBuffer(GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat,
	GLuint vFormat, GLuint vType)
{
	bind();
	std::shared_ptr<CGBuffer> gBuffer = std::make_shared<CGBuffer>(vTextureUnit, 
		vColorAttachment, vInternalFormat, vFormat, vType, m_Width, m_Height);
	m_GBuffers.insert(gBuffer);
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
	}
	glDrawBuffers(Attachments.size(), Attachments.data());

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		HIVE_LOG_ERROR("Framebuffer not complete!");

	// 创建并附加深度缓冲
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFramebuffer::__createFramebuffer()
{
	CHECK_GL_ERROR(glGenFramebuffers(1, &m_Framebuffer));
}
