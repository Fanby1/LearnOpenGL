#include "CVertexBufferObject.h"
#include <numeric>

CVertexBufferObject::CVertexBufferObject(float* vVertices, size_t vSize, unsigned int vType, const std::vector<unsigned int>& vOffset)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, vSize, vVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_Type = vType;
	m_Offset = vOffset;
	m_Step = std::accumulate(m_Offset.begin(), m_Offset.end(), 0) * sizeof(float);
}

GLuint CVertexBufferObject::getID() const
{
	return m_ID;
}

unsigned int CVertexBufferObject::getType() const
{
	return m_Type;
}

const std::vector<unsigned int>& CVertexBufferObject::getOffset() const
{
	return m_Offset;
}

const void CVertexBufferObject::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	unsigned int Offset = 0;
	for (size_t i = 0; i < 4; i++) {
		if (m_Type & (1u << i)) {
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, m_Step, (void*)Offset);
			glEnableVertexAttribArray(i);
			Offset += m_Offset[i] * sizeof(float);
		}
	}
	return void();
}
