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
	_defineAttribute("POSITION", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
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
	hiveConfig::CHiveConfig* SCCamera = findSubconfigByName("CAMERA");
	hiveConfig::CHiveConfig* SCCameraPos = SCCamera->findSubconfigByName("POS");
	m_CameraPos(0) = SCCameraPos->getAttribute<float>("X").value();
	m_CameraPos(1) = SCCameraPos->getAttribute<float>("Y").value();
	m_CameraPos(2) = SCCameraPos->getAttribute<float>("Z").value();
	_setTypeVal<float>(m_FarPlane, "CAMERA|FAR");
	_setTypeVal<float>(m_NearPlane, "CAMERA|NEAR");
	_setTypeVal<float>(m_ViewField, "CAMERA|VIEWFIELD");
	_setTypeVal<std::string>(m_ModelPath, "MODEL|PATH");
}
