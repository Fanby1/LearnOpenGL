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
	void createAndAddGBuffer(GLuint vTextureUnit, GLuint vColorAttachment, GLuint vInternalFormat, GLuint vFormat, GLuint vType, unsigned int vWidth, unsigned int vHeight);
	void render() const;
private:
	GLuint m_Framebuffer;
	unsigned int m_Width;
	unsigned int m_Height;
	std::set<std::shared_ptr<CGBuffer>> m_GBuffers;
	void __createFramebuffer();
};

