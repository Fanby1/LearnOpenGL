#pragma once
#include "CRenderableObject.h"
#include "CPointLight.h"
class CStuff : public CRenderableObject
{
public:
    CStuff() = default;
    CStuff(const std::string& vPath);
    void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight) override;
};

