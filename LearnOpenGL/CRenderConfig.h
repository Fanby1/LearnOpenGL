#pragma once
#include <algorithm>
#include <set>
#include "CGLBaseConfig.h"

class GLTRAINVER3_API CRenderConfig : public CGLBaseConfig
{
public:
	CRenderConfig(const std::string& vFilePath = "");
	~CRenderConfig() = default;
	void initV() override;

	int getRenderPassNum() const
	{
		return m_RenderPasses.size();
	}
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
		USE_PER_VERTEX_SHADING,
	};
	ERenderPassType getRenderPassType(const int& vRenderPassIndex) const
	{
		int id = std::clamp(vRenderPassIndex, 0, (const int)m_RenderPasses.size() - 1);
		return m_RenderPasses[id]._RenderPassType;
	}

private:
	struct SRenderPass
	{
		int _VSIndex = -1, _FSIndex = -1;
		bool _isInit = false;
		ERenderPassType _RenderPassType = ERenderPassType::USE_PER_PIXEL_SHADING;
	};

	std::vector<SRenderPass> m_RenderPasses;
	std::vector<std::string> m_ShaderPathes;

	void __defineAttributesV() override;
	void __setValFromConfig();
};
