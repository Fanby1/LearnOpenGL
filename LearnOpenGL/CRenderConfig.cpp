#include "CRenderConfig.h"
#include "HiveLogger.h"
#include "HiveLogger.h"
#include "ConfigInterface.h"

CRenderConfig::CRenderConfig(const std::string& vFilePath)
{
	m_FilePath = vFilePath;
	__defineAttributesV();
	m_RenderPasses.clear();
	m_ShaderPathes.clear();
}

void CRenderConfig::init()
{
	if (m_FilePath.empty())
	{
		__logNoExist("**CONFIG FILE**");
	}
	__readConfigFromFile();
	__setValFromConfig();
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
	std::vector<hiveConfig::CHiveConfig*> RenderPassSubconfigs;
	extractSpecifiedSubconfigsRecursively("RENDER_PASS", RenderPassSubconfigs);
	if (RenderPassSubconfigs.empty()) 
	{
		__logNoExist("Render pass subconfig");
		return;
	}

	std::vector<hiveConfig::CHiveConfig*> ShadersSubconfigs;
	extractSpecifiedSubconfigsRecursively("SHADER", ShadersSubconfigs);
	std::vector<std::string> ShaderNames;
	ShaderNames.clear();
	for (const auto& SSubConfig : ShadersSubconfigs) 
	{
		std::string SName = SSubConfig->getName();
		bool ConfigHasPath = SSubConfig->getAttribute<std::string>("SHADER_SOURCE_FILE").has_value();
		if (!ConfigHasPath)
		{
			__logNoExist("Shader path in subconfig " + SName);
			continue;
		}
		std::string SPath = SSubConfig->getAttribute<std::string>("SHADER_SOURCE_FILE").value();
		ShaderNames.push_back(SName);
		m_ShaderPathes.push_back(SPath);
	}
	for (const auto& RSubconfig : RenderPassSubconfigs) 
	{
		SRenderPass RenderPass;
		std::string RName = RSubconfig->getName();
		//Messy code is found after getName() due to xml format. Use substring instead.
		if (RName.find("perpixel") != std::string::npos)
		{
			RenderPass._RenderPassType = ERenderPassType::USE_PER_PIXEL_SHADING;
		}
		else if (RName.find("pervertex") != std::string::npos)
		{
			RenderPass._RenderPassType = ERenderPassType::UES_PER_VERTEX_SHADING;
		}
		else
		{
			__logNoExist("Render pass type");
		}

		std::string VertexShaderName = RSubconfig->getAttribute<std::string>("VERTEX_SHADER").value();
		std::string FragmentShaderName = RSubconfig->getAttribute<std::string>("FRAGMENT_SHADER").value();
		HIVE_LOG_DEBUG(VertexShaderName + " v||f " + FragmentShaderName);
		for (int i = 0; i < ShaderNames.size(); ++i) 
		{
			if (ShaderNames[i].rfind(VertexShaderName, 0) == 0)
			{
				RenderPass._VSIndex = i;
			}
			else if (ShaderNames[i].rfind(FragmentShaderName, 0) == 0)
			{
				RenderPass._FSIndex = i;
			}
		}
		if (RenderPass._VSIndex >= 0 && RenderPass._FSIndex >= 0)
		{
			HIVE_LOG_INFO("Render pass {} in file {} is loaded.", RName, m_FilePath);
			RenderPass._isInit = true;
		}
		else
		{
			HIVE_LOG_WARNING("Renderer pass {} is not initialized!", RName);
			if (RenderPass._VSIndex < 0) __logNoExist("Vertex shader path of " + RName);
			if (RenderPass._FSIndex < 0) __logNoExist("Fragment shader path of " + RName);
			RenderPass._isInit = false;
		}
		m_RenderPasses.push_back(RenderPass);
	}
	return;
}

void CRenderConfig::__logNoExist(const std::string vVarName)
{
	HIVE_LOG_WARNING("{} is not read from XML file! Please check the format.", vVarName);
}