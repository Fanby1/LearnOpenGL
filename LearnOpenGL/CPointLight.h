#pragma once
#include "CRenderableObject.h"
class CPointLight : public CRenderableObject
{
	public:
	CPointLight() = default;
	CPointLight(const std::string& vPath);
	void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight) override;
};

