#pragma once
#include "CGLBaseConfig.h"
#include "ConfigInterface.h"
#include "HiveLogger.h"

CGLBaseConfig::CGLBaseConfig(const std::string& vFilePath) : m_FilePath(vFilePath) {}

void CGLBaseConfig::_logNoExist(const std::string vVarName)
{
	HIVE_LOG_WARNING("{} is not read from XML file! Please check the format.", vVarName);
}
