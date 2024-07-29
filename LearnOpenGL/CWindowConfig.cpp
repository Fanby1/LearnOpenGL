#include "CWindowConfig.h"
#include "HiveLogger.h"
#include "ConfigInterface.h"

CWindowConfig::CWindowConfig(const std::string& vFilePath)
	: CGLBaseConfig(vFilePath) 
{
	//Due to HiveConfig, must call _defineAttribute() during construction.
	__defineAttributesV();
	m_Width = m_Height = m_PosX = m_PosY = m_MajVer = m_MinVer = -1;
	m_IsCore = true;
	m_IsInit = false;
	m_Title = "";
}

void CWindowConfig::initV()
{
	if (m_FilePath.empty() || m_IsInit) 
	{
		_logNoExist("**EVERY CONFIG**");
		m_IsInit |= false;
	}
	hiveConfig::hiveParseConfig(m_FilePath, hiveConfig::EConfigType::XML, this);
	__setValFromConfig();
	m_IsInit = true;
}


void CWindowConfig::__defineAttributesV()
{
	//Due to HiveConfig, must have a copy&paste.
	_defineAttribute("Title", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("UseCoreProfile", hiveConfig::EConfigDataType::ATTRIBUTE_BOOL);
	_defineAttribute("MajorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("MinorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("Width", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("Height", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("PosX", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("PosY", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
}

void CWindowConfig::__setValFromConfig()
{
	_setTypeVal<std::string>(m_Title, "Title");
	_setTypeVal<bool>(m_IsCore, "UseCoreProfile");
	_setTypeVal<int>(m_MajVer, "MajorVersion");
	_setTypeVal<int>(m_MinVer, "MinorVersion");
	_setTypeVal<int>(m_Width, "Width");
	_setTypeVal<int>(m_Height, "Height");
	_setTypeVal<int>(m_PosX, "PosX");
	_setTypeVal<int>(m_PosY, "PosY");
}