#include "CFramebuffer.h"
#include <vector>
#include <HiveLogger.h>

CFramebuffer::CFramebuffer(unsigned int vWidth, unsigned int vHeight)
{
	m_Width = vWidth;
	m_Height = vHeight;
	__createFramebuffer();
}

void CFramebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void CFramebuffer::createAndAddGBuffer(GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat,
	GLuint vFormat, GLuint vType)
{
	bind();
	std::shared_ptr<CGBuffer> gBuffer = std::make_shared<CGBuffer>(vTextureUnit, 
		vColorAttachment, vInternalFormat, vFormat, vType, m_Width, m_Height);
	m_GBuffers.insert(gBuffer);
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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFramebuffer::__createFramebuffer()
{
	glGenFramebuffers(1, &m_Framebuffer);
}
