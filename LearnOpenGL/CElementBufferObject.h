#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class CElementBufferObject
{
private:
	GLuint m_ID = 0;
	unsigned int m_Size = 0;

public:
	CElementBufferObject(unsigned int* vIndices, size_t vSize);
	void bind();
	GLuint getID() const;
	size_t getSize() const;
};

