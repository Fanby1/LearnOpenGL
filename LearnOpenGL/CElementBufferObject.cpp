#include "CElementBufferObject.h"

CElementBufferObject::CElementBufferObject(unsigned int* vIndices, size_t vSize)
{
	glGenBuffers(1, &m_ID);
	// 绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

	m_Size = vSize / sizeof(GLuint);
	// 将索引数据传入EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vSize, vIndices, GL_STATIC_DRAW);
}

void CElementBufferObject::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

GLuint CElementBufferObject::getID() const
{
	return m_ID;
}

size_t CElementBufferObject::getSize() const
{
	return m_Size;
}
