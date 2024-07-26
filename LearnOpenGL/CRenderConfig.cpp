#include "CRenderConfig.h"
#include "HiveLogger.h"
#include "HiveLogger.h"
#include "ConfigInterface.h"

CRenderConfig::CRenderConfig(const std::string& vFilePath)
{
	m_FilePath = vFilePath;
	__defineAttributesV();
}

void CRenderConfig::init()
{
	if (m_FilePath.empty() || m_isInit)
	{
		__logNoExist("**EVERY CONFIG**");
		m_isInit |= false;
	}
	__readConfigFromFile();
	__setValFromConfig();
	m_isInit = true;
}

void CRenderConfig::__defineAttributesV()
{
	////Due to HiveConfig, must have a copy&paste.
	_defineAttribute("SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("SHADER_SOURCE_FILE", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("RENDER_ALGORITHM", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("RENDER_PASS", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("VERTEX_SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("FRAGMENT_SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
}

void CRenderConfig::__readConfigFromFile()
{
	hiveConfig::hiveParseConfig(m_FilePath, hiveConfig::EConfigType::XML, this);
}


void CRenderConfig::__setValFromConfig()
{
	//assert only one render pass provided in config.
	//if more, only use the first one.
	std::vector<hiveConfig::CHiveConfig*> RenderPassSubconfigs;
	extractSpecifiedSubconfigsRecursively("RENDER_PASS", RenderPassSubconfigs);
	if (RenderPassSubconfigs.empty()) 
	{
		__logNoExist("Render pass config");
		return;
	}
	m_RenderPass = RenderPassSubconfigs[0]->getName();
	//Messy code is found after getName() due to xml format. Use substring instead.
	if (m_RenderPass.find("perpixel") != std::string::npos) 
	{
		m_RenderPass = "perpixel_shading";
	}
	else if (m_RenderPass.find("pervertex") != std::string::npos) 
	{
		m_RenderPass = "pervertex_shading";
	}
	else
	{
		__logNoExist("Render pass value");
		m_RenderPass = "perpixel_shading";
	}

	std::string VertexShaderName = RenderPassSubconfigs[0]->getAttribute<std::string>("VERTEX_SHADER").value();
	std::string FragmentShaderName = RenderPassSubconfigs[0]->getAttribute<std::string>("FRAGMENT_SHADER").value();

	std::vector<hiveConfig::CHiveConfig*> ShadersSubconfigs;
	extractSpecifiedSubconfigsRecursively("SHADER", ShadersSubconfigs);
	for (auto i : ShadersSubconfigs) 
	{
		//Messy code is found after getName() due to xml format. Use substring instead.
		std::string t = i->getName();
		if (t.rfind(VertexShaderName, 0) == 0)
		{
			m_VSPathFound = true;
			m_VertexShaderPath = i->getAttribute<std::string>("SHADER_SOURCE_FILE").value();
		} 
		else if (t.rfind(FragmentShaderName, 0) == 0)
		{
			m_FSPathFound = true;
			m_FragmentShaderPath = i->getAttribute<std::string>("SHADER_SOURCE_FILE").value();
		}
	}

	if (m_VSPathFound && m_FSPathFound)
	{
		HIVE_LOG_INFO("Render Config is set from file {}", m_FilePath);
		m_isInit = true;
	}
	else
	{
		HIVE_LOG_WARNING("Renderer Config is not initialized!");
		if (!m_VSPathFound) __logNoExist("Vertex shader path");
		if (!m_FSPathFound) __logNoExist("Fragment shader path");
		m_isInit = false;
	}
}

template<typename T>
void CRenderConfig::__setTypeVal(T& vMemberName, const std::string& vVarName)
{
	std::optional<T> s;
	s.reset();
	s = getAttribute<T>(vVarName);
	if (!s.has_value()) __logNoExist(vVarName);
	else vMemberName = s.value();
}

void CRenderConfig::__logNoExist(const std::string vVarName)
{
	HIVE_LOG_WARNING("{} is not read from XML file! Please check the format.", vVarName);
}