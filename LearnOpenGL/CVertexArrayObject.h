#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <set>
#include <memory>
#include "CVertexBufferObject.h"
#include "CElementBufferObject.h"
#include "CShader.h"

class CVertexArrayObject
{
private:
	GLuint m_ID = 0;
	std::set<std::shared_ptr<CVertexBufferObject>> m_VBOs = {};
	std::shared_ptr<CElementBufferObject> m_EBO;
	bool __createVAO();

public:
	void addVBO(std::shared_ptr<CVertexBufferObject> vVBO);
	void deleteVBO(std::shared_ptr<CVertexBufferObject> vVBO);
	void setEBO(std::shared_ptr<CElementBufferObject> vEBO);
	void unsetEBO();
	GLuint getID() const;
	std::shared_ptr<CElementBufferObject> getEBO() const;
	std::set<std::shared_ptr<CVertexBufferObject>> getVBOs() const;
	void bind() const;

};

