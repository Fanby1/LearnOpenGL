#include "pch.h"
#include "CGlfwWindowConfig.h"

using namespace hiveConfig;
GLTrain::CGLFWWindowConfig::CGLFWWindowConfig()
{
	//_overwriteProductSig("GLFW_WINDOW_CONFIG");
	__setSelfWindowV();
}

void GLTrain::CGLFWWindowConfig::__setSelfWindowV()
{
	//_defineAttribute("WindowConfig", hiveConfig::EConfigDataType::ATTRIBUTE_ANON_SUBCONFIG);
	_defineAttribute("Width", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("Height", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("Title", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("MajorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("MinorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("UseCoreProfile", hiveConfig::EConfigDataType::ATTRIBUTE_BOOL);
	_defineAttribute("PosX", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
	_defineAttribute("PosY", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
}

void GLTrain::CGLFWWindowConfig::__loadDefaultWindowV()
{
	int MaxWidth = GetSystemMetrics(SM_CXSCREEN);
	int MaxHeight = GetSystemMetrics(SM_CYSCREEN);
	setAttribute("Width", MaxWidth >> 1);
	setAttribute("Height", MaxHeight >> 1);
	setAttribute("Title", std::string("Default Title"));
	setAttribute("MajorVersion", 4);
	setAttribute("MinorVersion", 6);
	setAttribute("UseCoreProfile", true);
	setAttribute("PosX", MaxWidth >> 2);
	setAttribute("PosY", MaxHeight >> 2);
}