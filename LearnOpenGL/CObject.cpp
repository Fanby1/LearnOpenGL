#include "CObject.h"

void CObject::createVAO()
{
	if (m_VAO != -1) {
		glDeleteVertexArrays(1, &m_VAO);
	}
	glCreateVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	for (auto VBO : m_VBO) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CObject::createVBO(std::shared_ptr<float> vVertices, size_t vSize, unsigned int vType)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, vVertices.get(), GL_STATIC_DRAW);
	int Step = 0;
	while (vType) {
		Step += vType & 1;
		vType >>= 1;
	}
	Step *= 3;
	for (size_t i = 0; i < 4; i++) {
		if (vType & (1u << i)) {
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, Step * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CObject::addVBO(GLuint vVBO)
{
	if (m_VBO.find(vVBO) != m_VBO.end()) {
		return;
	}
	m_VBO.insert(vVBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);
}
