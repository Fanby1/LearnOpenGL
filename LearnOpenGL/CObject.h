#pragma once
#define _USE_MATH_DEFINES
#include <map>
#include <memory>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <chrono>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "CVertexArrayObject.h"
#include "CShader.h"
#include "def.h"
#define VERTEX_TYPE_VERTEX_BIT 1
#define VERTEX_TYPE_COLOR_BIT 2
#define VERTEX_TYPE_TEXTURE_BIT 4
#define VERTEX_TYPE_NORMAL_BIT 8

class CObject
{
public:
	CObject() = default;
	void setUpdateMoveFunction(std::function<void(std::chrono::duration<double>, CObject&)> vFunction);
	void move(Eigen::Vector3f vDisplacement);
	void setPosition(Eigen::Vector3f vNewPosition);
	void scale(float vScale);
	void setScale(float vScale);
	Eigen::Vector3f getPosition();

protected:
	Eigen::Vector3f m_Position = { 0,0,0 };
	float m_Scale = 1.0f;

	// 成员变量，存储 std::function
	std::function<void(std::chrono::duration<double>, CObject&)> m_UpdateMoveFunction;

	// 检测 myFunction 是否为空
	bool __isFunctionSet() const {
		return static_cast<bool>(m_UpdateMoveFunction);
	}

	std::chrono::steady_clock::time_point m_Start = std::chrono::high_resolution_clock::now();
	void __transform(std::shared_ptr<CShader> vShader);
	void __rotation(std::shared_ptr<CShader> vShader);
};
