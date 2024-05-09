#pragma once
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define VERTEX_TYPE_VERTEX_BIT 1
#define VERTEX_TYPE_COLOR_BIT 2
#define VERTEX_TYPE_TEXTURE_BIT 4
#define VERTEX_TYPE_NORMAL_BIT 8
class CObject
{
private:
	GLuint m_VAO = -1;
	std::map<GLuint, std::pair<unsigned int, std::vector<unsigned int>>> m_VBO;

	GLuint m_EBO = -1;
	unsigned int m_VeticesOffset, m_ColorsOffset, m_TexturesOffset;
	void __setPointer(unsigned int vType, std::vector<unsigned> vOffset);
public:
	
	void createVAO();
	GLuint createVBO(float* vVertices, size_t vSize, unsigned int vType, std::vector<unsigned int> vOffset);
	void addVBO(GLuint vVBO, unsigned int vType, std::vector<unsigned int> vOffset);
	GLuint createEBO(unsigned int* vIndices, size_t vSize);
	void deleteVBO(GLuint vVBO);
	void setEBO(GLuint vEBO);
	GLuint getVAO();
};

