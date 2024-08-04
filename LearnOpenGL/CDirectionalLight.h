#pragma once
#include <chrono>
#include "def.h"
#include <Eigen/Core>
#include "CShader.h"
#include "CLight.h"

class GLTRAINVER3_API CDirectionalLight : public CLight
{
public:
	CDirectionalLight() = default;
	~CDirectionalLight() = default;
	const Eigen::Vector3f& getDirection() const;
	const Eigen::Vector3f& getAmbient() const;
	const Eigen::Vector3f& getDiffuse() const;
	const Eigen::Vector3f& getSpecular() const;
	void updateShaderUniforms(const std::shared_ptr<CShader>& vShader);
	void rotate(float vAngle, const Eigen::Vector3f& vAxis);
private:
	std::chrono::steady_clock::time_point m_Start = std::chrono::high_resolution_clock::now();
	Eigen::Vector3f m_Direction = { -0.2f, -1.0f, -0.3f };
	Eigen::Vector3f m_Ambient = { 0.2f, 0.2f, 0.2f };
	Eigen::Vector3f m_Diffuse = { 0.5f, 0.5f, 0.5f };
	Eigen::Vector3f m_Specular = { 1.0f, 1.0f, 1.0f };
};

