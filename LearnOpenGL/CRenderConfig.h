#pragma once
#include <string>
#include "HiveConfig.h"

class CRenderConfig : protected hiveConfig::CHiveConfig 
{
public:
	CRenderConfig(const std::string& vFilePath = "");
	~CRenderConfig() = default;
	const bool isInit() { return m_isInit; }
	void init();
	const std::string getVertexShaderPath() { return m_VertexShaderPath; }
	const std::string getFragmentShaderPath() { return m_FragmentShaderPath; }
	const bool isUsingPerVertexShading() { return m_UsePerVertexShading; }

private:
	std::string m_FilePath = "";
	std::string m_VertexShaderPath = "", m_FragmentShaderPath = "";
	bool m_UsePerVertexShading = false;
	bool m_isInit = false;
	bool m_VSPathFound = false, m_FSPathFound = false;

	void __defineAttributesV();
	void __readConfigFromFile();
	void __setValFromConfig();
	template<typename T>void __setTypeVal(T& vMemberName, const std::string& vType);
	void __logNoExist(const std::string vType);
};
