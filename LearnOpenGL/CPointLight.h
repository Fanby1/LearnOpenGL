#pragma once
#include "CLight.h"
class CPointLight : public CLight
{
	public:
	CPointLight() = default;
	CPointLight(const std::string& vPath);
	void updateShaderUniforms(std::shared_ptr<CShader> vShader);
};

