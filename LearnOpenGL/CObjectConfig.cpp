#include "CObjectConfig.h"
#include "HiveLogger.h"
#include "ConfigInterface.h"

CObjectConfig::CObjectConfig(const std::string& vFilePath)
	: CGLBaseConfig(vFilePath)
{
	__defineAttributesV();
}

void CObjectConfig::initV()
{
	if (m_FilePath.empty())
	{
		_logNoExist("**OBJECT CONFIG FILE**");
		m_IsInit |= false;
	}
	hiveConfig::hiveParseConfig(m_FilePath, hiveConfig::EConfigType::XML, this);
	__setValFromConfig();
	m_IsInit = true;
}

void CObjectConfig::__defineAttributesV()
{
	_defineAttribute("CAMERA", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("X", hiveConfig::EConfigDataType::ATTRIBUTE_FLOAT);
	_defineAttribute("Y", hiveConfig::EConfigDataType::ATTRIBUTE_FLOAT);
	_defineAttribute("Z", hiveConfig::EConfigDataType::ATTRIBUTE_FLOAT);
	_defineAttribute("NEAR", hiveConfig::EConfigDataType::ATTRIBUTE_FLOAT);
	_defineAttribute("FAR", hiveConfig::EConfigDataType::ATTRIBUTE_FLOAT);
	_defineAttribute("VIEWFIELD", hiveConfig::EConfigDataType::ATTRIBUTE_FLOAT);

	_defineAttribute("MODEL", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("PATH", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
}

void CObjectConfig::__setValFromConfig()
{
	//subconfig
	std::vector<hiveConfig::CHiveConfig*> SCCamera;
	extractSpecifiedSubconfigsRecursively("CAMERA", SCCamera);
	float x, y, z;
	_setTypeVal<float>(x,"X",SCCamera[0]);
	_setTypeVal<float>(y,"Y",SCCamera[0]);
	_setTypeVal<float>(z,"Z",SCCamera[0]);
	m_CameraPos = Eigen::Vector3f(x, y, z);
	_setTypeVal<float>(m_FarPlane, "FAR", SCCamera[0]);
	_setTypeVal<float>(m_NearPlane, "NEAR", SCCamera[0]);
	_setTypeVal<float>(m_ViewField, "VIEWFIELD", SCCamera[0]);
	SCCamera.clear();
	extractSpecifiedSubconfigsRecursively("MODEL", SCCamera);
	_setTypeVal<std::string>(m_ModelPath, "PATH", SCCamera[0]);
}
