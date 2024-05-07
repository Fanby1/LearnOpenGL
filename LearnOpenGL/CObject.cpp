#include "CObject.h"

void CObject::__setPointer(unsigned int vType)
{
	int Step = 0;
	auto Temp = vType;
	while (Temp) {
		Step += Temp & 1;
		Temp >>= 1;
	}
	Step *= 3;
	unsigned int Offset = 0;
	for (size_t i = 0; i < 4; i++) {
		if (vType & (1u << i)) {
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, Step * sizeof(float), (void*)Offset);
			glEnableVertexAttribArray(i);
			Offset += 3 * sizeof(float);
		}
	}
}

void CObject::createVAO()
{
	if (m_VAO != -1) {
		glDeleteVertexArrays(1, &m_VAO);
	}
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	for (auto& VBO : m_VBO) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO.first);
		__setPointer(VBO.second);
	}
	if (m_EBO != -1) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint CObject::createVBO(float* vVertices, size_t vSize, unsigned int vType)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, vVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_VBO[VBO] = vType;
	return VBO;
}

void CObject::addVBO(GLuint vVBO, unsigned int vType)
{
	if (m_VBO.find(vVBO) != m_VBO.end()) {
		return;
	}
	m_VBO[vVBO] = vType;
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);
	createVAO();
}

void CObject::deleteVBO(GLuint vVBO)
{
	m_VBO.erase(vVBO);
	createVAO();
}

GLuint CObject::createEBO(unsigned int* vIndices, size_t vSize)
{
	glGenBuffers(1, &m_EBO);
	// 绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	// 将索引数据传入EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vSize, vIndices, GL_STATIC_DRAW);
	return m_EBO;
}

void CObject::setEBO(GLuint vEBO)
{
	m_EBO = vEBO;
	createVAO();
}

GLuint CObject::getVAO()
{
	return m_VAO;
}
