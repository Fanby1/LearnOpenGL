#pragma once
#include <vector>
#include <memory>
#include <tiny_gltf.h>
#include "CRenderableObject.h"
#include "CTexture.h"

class GLTRAINVER3_API CGLTFObject :
    public CRenderableObject
{
public:
    CGLTFObject() = default;
    void loadModelFromGLTF(const std::string& vPath);
private:
    std::vector<std::shared_ptr<CTexture>> m_Textures;
    std::vector<std::shared_ptr<CImage>> m_Images;
    void __printAttributes(const tinygltf::Primitive& vPrimitive);
    void __printTextureInfo(const tinygltf::Model& vModel);
    void __loadTextures(const tinygltf::Model& vModel);
    void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight) override;
};

