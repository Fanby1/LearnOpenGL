#include "CDirectionalLight.h"
#include "iostream"
#include "HiveLogger.h"

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
	if (vShader == nullptr) return;
	if(__isFunctionSet()) {
		auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
	}
	vShader->setVec3(m_NameInShader + ".direction", m_Direction);
	vShader->setVec3(m_NameInShader + ".ambient", m_Ambient);
	vShader->setVec3(m_NameInShader + ".diffuse", m_Diffuse);
	vShader->setVec3(m_NameInShader + ".specular", m_Specular);
}

void CDirectionalLight::rotate(float vAngle, const Eigen::Vector3f& vAxis)
{
	Eigen::Vector3f Temp = vAxis;
	Temp.normalize();
	Eigen::AngleAxisf Rotation(vAngle, Temp);
	m_Direction = Rotation.toRotationMatrix() * m_Direction;
}
