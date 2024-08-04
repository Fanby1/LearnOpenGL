#pragma once
#include "CObject.h"
#include "def.h"

class GLTRAINVER3_API CLight : public CObject
{
	/*
	颜色（Color）：

表示光源的颜色，一般使用RGB值。
强度（Intensity）：

光源的强度或亮度。
	*/
public:
	CLight() = default;
	void setColor(const Eigen::Vector3f& vColor);
	void setIntensity(float vIntensity);
	const Eigen::Vector3f& getColor() const;
	float getIntensity() const;
protected:
	Eigen::Vector3f m_Color = { 1.0f, 1.0f, 1.0f };
	float m_Intensity = 1.0f;
};

