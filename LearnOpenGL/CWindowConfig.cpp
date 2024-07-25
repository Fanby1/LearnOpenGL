#include "CWindowConfig.h"
#include "HiveLogger.h"
#include "ConfigInterface.h"

CWindowConfig::CWindowConfig(const std::string& vFilePath)
{
	m_FilePath = vFilePath; 
	__defineAttributesV();
	m_isInit = false;
}

void CWindowConfig::init()
{
	if (m_FilePath.empty() || m_isInit) {
		__logNoExist("**EVERYTHING**");
		m_isInit |= false;
	}
	__readConfigFromFile();
	__setValFromConfig();
	m_isInit = true;
}

void CWindowConfig::__readConfigFromFile()
{
	hiveConfig::hiveParseConfig(m_FilePath, hiveConfig::EConfigType::XML, this);
}

void CWindowConfig::__defineAttributesV()
{
	_defineAttribute("Width", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("Height", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("Title", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("MajorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("MinorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("UseCoreProfile", hiveConfig::EConfigDataType::ATTRIBUTE_BOOL);
	_defineAttribute("PosX", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("PosY", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
}

void CWindowConfig::__setValFromConfig()
{
	__setTypeVal<int>(m_Width, "Width");
	__setTypeVal<int>(m_Height, "Height");
	__setTypeVal<int>(m_PosX, "PosX");
	__setTypeVal<int>(m_PosY, "PosY");
	__setTypeVal<int>(m_MajVer, "MajorVersion");
	__setTypeVal<int>(m_MinVer, "MinorVersion");
	__setTypeVal<std::string>(m_Title, "Title");
	__setTypeVal<bool>(m_isCore, "USE_GLFW_OPENGL_CORE_PROFILE");
}

template<typename T>
void CWindowConfig::__setTypeVal(T& vMemberName, const std::string& vVarName)
{
	std::optional<T> s;
	s.reset();
	s = getAttribute<T>(vVarName);
	if (!s.has_value()) __logNoExist(vVarName);
	else vMemberName = s.value();
}

void CWindowConfig::__logNoExist(const std::string vVarName)
{
	HIVE_LOG_WARNING("{} is not read from XML file! Please check the format.", vVarName);
}