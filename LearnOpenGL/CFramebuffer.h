#pragma once
#include <set>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CGBuffer.h"
#include "def.h";

class GLTRAINVER3_API CFramebuffer
{
public:
	CFramebuffer(unsigned int vWidth, unsigned int vHeight);
	void bind() const;
	void createAndAddGBuffer(const std::string& vUniformName, GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat, GLuint vFormat, GLuint vType);
	void createAndAddDepthBuffer(const std::string& vUniformName, GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat, GLuint vFormat, GLuint vType);
	const std::set<std::shared_ptr<CGBuffer>> getGBuffers() const;
	const std::shared_ptr<CGBuffer> getDepthBuffer() const { return m_DepthBuffer; }
	void render() const;
private:
	GLuint m_Framebuffer;
	unsigned int m_Width;
	unsigned int m_Height;
	std::set<std::shared_ptr<CGBuffer>> m_GBuffers;
	std::shared_ptr<CGBuffer> m_DepthBuffer;
	void __createFramebuffer();
};

