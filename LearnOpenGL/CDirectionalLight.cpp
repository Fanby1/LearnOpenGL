#include "CDirectionalLight.h"
#include "iostream"

void CDirectionalLight::setDirection(const Eigen::Vector3f& vDirection)
{
	m_Direction = vDirection;
}

void CDirectionalLight::setAmbient(const Eigen::Vector3f& vAmbient)
{
	m_Ambient = vAmbient;
}

void CDirectionalLight::setDiffuse(const Eigen::Vector3f& vDiffuse)
{
	m_Diffuse = vDiffuse;
}

void CDirectionalLight::setSpecular(const Eigen::Vector3f& vSpecular)
{
	m_Specular = vSpecular;
}

const Eigen::Vector3f& CDirectionalLight::getDirection() const
{
	return m_Direction;
}

const Eigen::Vector3f& CDirectionalLight::getAmbient() const
{
	return m_Ambient;
}

const Eigen::Vector3f& CDirectionalLight::getDiffuse() const
{
	return m_Diffuse;
}

const Eigen::Vector3f& CDirectionalLight::getSpecular() const
{
	return m_Specular;
}

void CDirectionalLight::updateShaderUniforms(const std::shared_ptr<CShader>& vShader)
{
	if(__isFunctionSet()) {
		auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
	}
	vShader->setVec3("light.direction", m_Direction);
	vShader->setVec3("light.ambient", m_Ambient);
	vShader->setVec3("light.diffuse", m_Diffuse);
	vShader->setVec3("light.specular", m_Specular);
}

void CDirectionalLight::setUpdateMoveFunction(std::function<void(std::chrono::duration<double>, CDirectionalLight&)> vFunction)
{
	m_UpdateMoveFunction = vFunction;
}

void CDirectionalLight::rotate(float vAngle, const Eigen::Vector3f& vAxis)
{
	Eigen::Vector3f Temp = vAxis;
	Temp.normalize();
	Eigen::AngleAxisf Rotation(vAngle, Temp);
	m_Direction = Rotation * m_Direction;
}
