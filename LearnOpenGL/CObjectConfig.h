#pragma once
#include <Eigen/Dense>
#include "CGLBaseConfig.h"

class GLTRAINVER3_API CObjectConfig : public CGLBaseConfig
{
public:
	CObjectConfig(const std::string& vFilePath = "");
	~CObjectConfig() = default;
	void initV() override;
	bool isInit() const { return m_IsInit; }
	Eigen::Vector3f getCameraPos() const { return m_CameraPos; }
	float getFarPlane() const { return m_FarPlane; }
	float getNearPlane() const { return m_NearPlane; }
	float getViewField() const { return m_ViewField; }
	std::string getModelPath() const { return m_ModelPath; }

private:
	void __defineAttributesV();
	void __setValFromConfig();
	std::string m_ModelPath = "";
	bool m_IsInit = false;
	Eigen::Vector3f m_CameraPos = Eigen::Vector3f(0, 0, 0);
	float m_FarPlane = 100.0f, m_NearPlane = 0.1f, m_ViewField = 45.0;
};
