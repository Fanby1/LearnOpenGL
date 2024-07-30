#include "CRenderConfig.h"
#include "HiveLogger.h"
#include "ConfigInterface.h"

CRenderConfig::CRenderConfig(const std::string& vFilePath)
	: CGLBaseConfig(vFilePath)
{
	__defineAttributesV();
	m_RenderPasses.clear();
	m_ShaderPathes.clear();
}

void CRenderConfig::initV ()
{
	if (m_FilePath.empty())
	{
		_logNoExist("**RENDER CONFIG FILE**");
	}
	hiveConfig::hiveParseConfig(m_FilePath, hiveConfig::EConfigType::XML, this);
	__setValFromConfig();
}

void CRenderConfig::__defineAttributesV()
{
	_defineAttribute("SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("SHADER_SOURCE_FILE", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("RENDER_ALGORITHM", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("RENDER_PASS", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("GEOMETRY_PROGRAM", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("LIGHTING_PROGRAM", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("VERTEX_SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("FRAGMENT_SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("UNIFORM", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
	_defineAttribute("UNITYPE", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	_defineAttribute("UNIVALUE", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
}

void CRenderConfig::__setValFromConfig()
{
	std::vector<hiveConfig::CHiveConfig*> ShadersSubconfigs;
	extractSpecifiedSubconfigsRecursively("SHADER", ShadersSubconfigs);
	std::vector<std::string> ShaderNames;
	ShaderNames.clear();
	for (const auto& SSubConfig : ShadersSubconfigs) 
	{
		std::string SName = SSubConfig->getName();
		SName.erase(SName.find_last_of(' '));
		bool ConfigHasPath = SSubConfig->getAttribute<std::string>("SHADER_SOURCE_FILE").has_value();
		if (!ConfigHasPath)
		{
			_logNoExist("Shader path in subconfig " + SName);
			continue;
		}
		ShaderNames.push_back(SName);
		std::string SPath = SSubConfig->getAttribute<std::string>("SHADER_SOURCE_FILE").value();
		m_ShaderPathes.push_back(SPath);
	}

	std::vector<hiveConfig::CHiveConfig*> RenderPassSubconfigs;
	extractSpecifiedSubconfigsRecursively("GEOMETRY_PROGRAM", RenderPassSubconfigs);
	extractSpecifiedSubconfigsRecursively("LIGHTING_PROGRAM", RenderPassSubconfigs);
	//help xml make sure Geo before Light
	if (RenderPassSubconfigs.empty())
	{
		_logNoExist("Render pass subconfig");
		return;
	}
	for (const auto& RSubconfig : RenderPassSubconfigs) 
	{
		SRenderPass RenderPass;
		RenderPass._Uniforms.clear();
		std::string RName = RSubconfig->getName();
		RName.erase(RName.find_first_of(' '));

		std::string VertexShaderName = RSubconfig->getAttribute<std::string>("VERTEX_SHADER").value();
		std::string FragmentShaderName = RSubconfig->getAttribute<std::string>("FRAGMENT_SHADER").value();

		for (int i = 0; i < ShaderNames.size(); ++i) 
		{
			if (ShaderNames[i] == VertexShaderName)
			{
				RenderPass._VSIndex = i;
			}
			else if (ShaderNames[i] == FragmentShaderName)
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
			if (RenderPass._VSIndex < 0) _logNoExist("Vertex shader path of " + RName);
			if (RenderPass._FSIndex < 0) _logNoExist("Fragment shader path of " + RName);
			RenderPass._isInit = false;
		}
		__setUniform(RSubconfig, &RenderPass);

		m_RenderPasses.push_back(RenderPass);
	}
	return;
}

void CRenderConfig::__setUniform(hiveConfig::CHiveConfig* vShadingProgramSubConfig, SRenderPass* voRenderPass)
{
	std::vector<hiveConfig::CHiveConfig*> UniformSubconfigs;
	vShadingProgramSubConfig->extractSpecifiedSubconfigsRecursively("UNIFORM", UniformSubconfigs);
	for (const auto& USubConfig : UniformSubconfigs) 
	{
		SUniform Tmp;
		Tmp._UName = USubConfig->getName();
		Tmp._UType = USubConfig->getAttribute<std::string>("UNITYPE").value();
		Tmp._UValue = USubConfig->getAttribute<std::string>("UNIVALUE").value();
		voRenderPass->_Uniforms.push_back(Tmp);
	}
}
