#pragma once
#include "CObject.h"
#include "def.h"

class GLTRAINVER3_API CLight : public CObject
{
	/*
	��ɫ��Color����

��ʾ��Դ����ɫ��һ��ʹ��RGBֵ��
ǿ�ȣ�Intensity����

��Դ��ǿ�Ȼ����ȡ�
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

