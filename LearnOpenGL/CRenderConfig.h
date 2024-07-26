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
	std::string const getVertexShaderPath() { return m_VertexShaderPath; }
	std::string const getFragmentShaderPath() { return m_FragmentShaderPath; }
	std::string const getRenderPass() { return m_RenderPass; }

private:
	std::string m_FilePath = "";
	std::string m_VertexShaderPath = "", m_FragmentShaderPath = "";
	std::string m_RenderPass = "";
	bool m_isInit = false;
	bool m_VSPathFound = false, m_FSPathFound = false;

	void __defineAttributesV();
	void __readConfigFromFile();
	void __setValFromConfig();
	template<typename T>void __setTypeVal(T& vMemberName, const std::string& vType);
	void __logNoExist(const std::string vType);
};
