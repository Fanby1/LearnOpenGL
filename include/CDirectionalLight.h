#pragma once
#include <chrono>
#include <Eigen/Core>
#include "CShader.h"

class CDirectionalLight
{
public:
	CDirectionalLight() = default;
	~CDirectionalLight() = default;
	const Eigen::Vector3f& getDirection() const;
	const Eigen::Vector3f& getAmbient() const;
	const Eigen::Vector3f& getDiffuse() const;
	const Eigen::Vector3f& getSpecular() const;
	void updateShaderUniforms(const std::shared_ptr<CShader>& vShader);
	void setUpdateMoveFunction(std::function<void(std::chrono::duration<double>, CDirectionalLight&)> vFunction);
	void rotate(float vAngle, const Eigen::Vector3f& vAxis);
private:
	std::chrono::steady_clock::time_point m_Start = std::chrono::high_resolution_clock::now();
	Eigen::Vector3f m_Direction = { -0.2f, -1.0f, -0.3f };
	Eigen::Vector3f m_Ambient = { 0.8f, 0.8f, 0.8f };
	Eigen::Vector3f m_Diffuse = { 0.5f, 0.5f, 0.5f };
	Eigen::Vector3f m_Specular = { 1.0f, 1.0f, 1.0f };
	// 成员变量，存储 std::function
	std::function<void(std::chrono::duration<double>, CDirectionalLight&)> m_UpdateMoveFunction;

	// 检测 myFunction 是否为空
	bool __isFunctionSet() const {
		return static_cast<bool>(m_UpdateMoveFunction);
	}
};

