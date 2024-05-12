#pragma once
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CVertexArrayObject.h"
#include "CShader.h"

#define VERTEX_TYPE_VERTEX_BIT 1
#define VERTEX_TYPE_COLOR_BIT 2
#define VERTEX_TYPE_TEXTURE_BIT 4
#define VERTEX_TYPE_NORMAL_BIT 8


class CObject
{
private:
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>> m_VAOs;
public:
	void addVAO(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader);
	void deleteVAO(std::shared_ptr<CVertexArrayObject> vVAO);
	void clearVAO();
	void setVAO(std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>&& vVAOs);
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator begin();
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator end();
};
