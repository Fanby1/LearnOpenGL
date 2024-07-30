#include "CVertexArrayObject.h"

bool CVertexArrayObject::__createVAO()
{
	if (m_ID != 0) 
	{
		glDeleteVertexArrays(1, &m_ID);
	}
	glGenVertexArrays(1, &m_ID);
	glBindVertexArray(m_ID);
	for (const auto& VBO : m_VBOs) 
	{
		VBO->bind();
	}
	if (m_EBO != nullptr) 
	{
		m_EBO->bind();
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    return false;
}

void CVertexArrayObject::addVBO(std::shared_ptr<CVertexBufferObject> vVBO)
{
	auto VBO = m_VBOs.find(vVBO);
	if (VBO == m_VBOs.end()) 
	{
		m_VBOs.insert(vVBO);
		__createVAO();
	}
}

void CVertexArrayObject::deleteVBO(std::shared_ptr<CVertexBufferObject> vVBO)
{
	auto VBO = m_VBOs.find(vVBO);
	if (VBO != m_VBOs.end()) 
	{
		m_VBOs.erase(VBO);
		__createVAO();
	}
}

void CVertexArrayObject::setEBO(std::shared_ptr<CElementBufferObject> vEBO)
{
	m_EBO = vEBO;
	__createVAO();
}

void CVertexArrayObject::unsetEBO()
{
	m_EBO = nullptr;
	__createVAO();
}

GLuint CVertexArrayObject::getID() const
{
	return m_ID;
}

std::shared_ptr<CElementBufferObject> CVertexArrayObject::getEBO() const
{
	return m_EBO;
}

std::set<std::shared_ptr<CVertexBufferObject>> CVertexArrayObject::getVBOs() const
{
	return m_VBOs;
}

void CVertexArrayObject::bind() const
{
	glBindVertexArray(m_ID);
}
