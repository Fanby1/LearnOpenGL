#pragma once
#include "CRenderableObject.h"
#include "CLight.h"
class CStuff :
    public CRenderableObject
{
public:
    CStuff() = default;
    CStuff(const std::string& vPath, std::shared_ptr<CShader> vShader);
    void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CLight> vLight) override;
};

