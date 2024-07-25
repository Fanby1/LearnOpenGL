#pragma once
#include "CObject.h"
class CLight : CObject
{
	public:
	CLight() = default;
	CLight(const std::string& vPath, std::shared_ptr<CShader> vShader);
};

