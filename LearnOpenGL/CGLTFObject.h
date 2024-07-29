#pragma once
#include <HiveLogger.h>
#include <memory>
#include <tiny_gltf.h>
#include <vector>
#include "CPointLight.h"
#include "CRenderableObject.h"
#include "CShader.h"
#include "CTexture.h"
#include "CGBuffer.h"
#include "def.h"

class GLTRAINVER3_API CGLTFObject :
    public CRenderableObject
{
public:
    CGLTFObject() = default;
    CGLTFObject(const std::string& vPath);
    void loadModelFromGLTF(const std::string& vPath);
    void setShader(std::shared_ptr<CShader> vShader) 
    {
		m_Shader = vShader;
	}
private:
    std::vector<std::shared_ptr<CTexture>> m_Textures;
    std::vector<std::shared_ptr<CImage>> m_Images;
    std::shared_ptr<CShader> m_Shader;
    void __printAndLoadAttributes(const tinygltf::Model& vModel, const tinygltf::Primitive& vPrimitive);
    const unsigned char* __getDataPointer(const tinygltf::Model& vModel, const tinygltf::Accessor& vAccessor);
    void __printBufferView(const tinygltf::Model& vModel, int vBufferViewIndex);
    void __extractIndices(const tinygltf::Model& vModel, std::vector<unsigned int>& vIndices);
    void __printTextureInfo(const tinygltf::Model& vModel);
    void __loadTextures(const tinygltf::Model& vModel);
    void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight) override;
    void renderGeometryV(std::shared_ptr<CCamera> vCamera) override;
    template<typename T>
    void __rearrangeArray(const T* vArray, size_t vLength, size_t vStep, std::vector<T>& vTargetArray, int vInterval, int vOffset);
};
template<typename T>
void CGLTFObject::__rearrangeArray(const T* vArray, size_t vLength, size_t vStep, std::vector<T>& vTargetArray, int vInterval, int vOffset) {
    // make sure transform no excceed
    vOffset = vOffset % vInterval;
    auto Position = vArray;
    int Index = vOffset;
    while (Position - vArray < vLength) {
        for (int i = 0; i < vStep; i++, Index++, Position++) {
            vTargetArray[Index] = *Position;
        }
        Index += vInterval - vStep;
    }
}
