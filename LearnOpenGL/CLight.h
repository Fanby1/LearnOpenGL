#pragma once
#include "CObject.h"
class CLight : public CObject
{
	/*
	颜色（Color）：

表示光源的颜色，一般使用RGB值。
强度（Intensity）：

光源的强度或亮度。
	*/
public:

protected:
	Eigen::Vector3f m_Color = { 1.0f, 1.0f, 1.0f };
	float m_Intensity = 1.0f;
};

