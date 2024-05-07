#pragma once
#include <set>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class CObject
{
private:
	std::shared_ptr<float> m_Vertices = nullptr;
	std::shared_ptr<GLuint> m_Indices = nullptr;
	GLuint m_VAO = -1;
	std::set<GLuint> m_VBO;
	GLuint m_EBO = -1;
	unsigned int m_VeticesOffset, m_ColorsOffset, m_TexturesOffset;
public:
	void createVAO();
	void createVBO(std::shared_ptr<float> vVertices, size_t vSize, unsigned int vType);
	void addVBO(GLuint vVBO);
	void deleteVBO(GLuint vVBO);
	void createEBO(std::shared_ptr<unsigned int> vIndices, size_t vSize);
	void setEBO(GLuint vEBO);
};

