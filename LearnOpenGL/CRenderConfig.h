#pragma once
#include <algorithm>
#include <string>
#include <set>
#include "def.h"
#include "HiveConfig.h"

class GLTRAINVER3_API CRenderConfig : protected hiveConfig::CHiveConfig
{
public:

	CRenderConfig(const std::string& vFilePath = "");
	~CRenderConfig() = default;
	void init();

	bool isInit(const int& vRenderPassIndex) const 
	{ 
		int id = std::clamp(vRenderPassIndex, 0, (const int)m_RenderPasses.size() - 1);
		return m_RenderPasses[id]._isInit;
	}
	std::string getVertexShaderPath(const int& vRenderPassIndex) const
	{ 
		int id = std::clamp(vRenderPassIndex, 0, (const int)m_RenderPasses.size() - 1);
		if (m_RenderPasses[id]._VSIndex >= 0) return m_ShaderPathes[m_RenderPasses[id]._VSIndex];
		else return "";

	}
	std::string getFragmentShaderPath(const int& vRenderPassIndex) const
	{
		int id = std::clamp(vRenderPassIndex, 0, (const int)m_RenderPasses.size() - 1);
		if (m_RenderPasses[id]._FSIndex >= 0) return m_ShaderPathes[m_RenderPasses[id]._FSIndex];
		else return "";
	}
	enum class ERenderPassType : unsigned char
	{
		USE_PER_PIXEL_SHADING = 0,
		UES_PER_VERTEX_SHADING,
	};
	ERenderPassType getRenderPassType(const int& vRenderPassIndex) const
	{
		int id = std::clamp(vRenderPassIndex, 0, (const int)m_RenderPasses.size() - 1);
		return m_RenderPasses[id]._RenderPassType;
	}


private:
	std::string m_FilePath = "";

	struct SRenderPass
	{
		ERenderPassType _RenderPassType = ERenderPassType::USE_PER_PIXEL_SHADING;
		int _VSIndex = -1, _FSIndex = -1;
		bool _isInit = false;
	};

	std::vector<SRenderPass> m_RenderPasses;
	std::vector<std::string> m_ShaderPathes;

	void __defineAttributesV();
	void __readConfigFromFile();
	void __setValFromConfig();
	void __logNoExist(const std::string vType);
};
