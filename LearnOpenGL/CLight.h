#pragma once
#include "CObject.h"
class CLight : public CObject
{
	/*
	��ɫ��Color����

��ʾ��Դ����ɫ��һ��ʹ��RGBֵ��
ǿ�ȣ�Intensity����

��Դ��ǿ�Ȼ����ȡ�
	*/
public:

protected:
	Eigen::Vector3f m_Color = { 1.0f, 1.0f, 1.0f };
	float m_Intensity = 1.0f;
};

