#pragma once
#include "CRenderableObject.h"
class CLight : public CRenderableObject
{
	public:
	CLight() = default;
	CLight(const std::string& vPath, std::shared_ptr<CShader> vShader);
	void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CLight> vLight) override;
};

