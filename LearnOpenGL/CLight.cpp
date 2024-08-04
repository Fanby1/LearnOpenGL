#include "CLight.h"

void CLight::setColor(const Eigen::Vector3f& vColor)
{
	m_Color = vColor;
}

void CLight::setIntensity(float vIntensity)
{
	m_Intensity = vIntensity;
}

const Eigen::Vector3f& CLight::getColor() const
{
	return m_Color;
}

float CLight::getIntensity() const
{
	return m_Intensity;
}
