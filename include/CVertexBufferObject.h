#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class CVertexBufferObject
{
private:
	GLuint m_ID = 0;
	unsigned int m_Type = 0;
	unsigned int m_Step = 0;
	unsigned int m_Size = 0;
	std::vector<unsigned int> m_Offset = {};
public:
	CVertexBufferObject(float* vVertices, size_t vSize, unsigned int vType, const std::vector<unsigned int>& vOffset);
	GLuint getID() const;
	unsigned int getType() const;
	const std::vector<unsigned int>& getOffset() const;
	const unsigned int getSize() const;
	const void bind() const;
};

