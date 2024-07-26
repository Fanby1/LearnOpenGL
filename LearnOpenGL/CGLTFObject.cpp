#include "CGLTFObject.h"
#include <HiveLogger.h>
#include <Singleton.h>
#include <tiny_gltf.h>


void CGLTFObject::loadModelFromGLTF(const std::string& vPath)
{
    auto loader = hiveCommon::CSingleton<tinygltf::TinyGLTF>::getInstance();
	tinygltf::Model Model;
	std::string ErroMessage;
	std::string WarningMessage;
	bool ReturnFlag = loader->LoadASCIIFromFile(&Model, &ErroMessage, &WarningMessage, vPath);

    if (!WarningMessage.empty()) 
    {
        HIVE_LOG_WARNING(WarningMessage);
    }
    if (!ErroMessage.empty()) 
    {
        HIVE_LOG_ERROR(ErroMessage);
    }
    if (!ReturnFlag) 
    {
        HIVE_LOG_ERROR("Failed to load GLTF file: {}", vPath);
        return;
    }

    // 输出模型信息
    HIVE_LOG_INFO("Model loaded successfully: {}", vPath);
    HIVE_LOG_INFO("Number of meshes: {}", Model.meshes.size());
    for (size_t i = 0; i < Model.meshes.size(); ++i) {
        const tinygltf::Mesh& Mesh = Model.meshes[i];
        HIVE_LOG_INFO("Mesh {} name: {}", i, Mesh.name);
        for (size_t j = 0; j < Mesh.primitives.size(); ++j) {
            const tinygltf::Primitive& Primitive = Mesh.primitives[j];
            HIVE_LOG_INFO("    Primitive {}", j);
            __printAttributes(Primitive);
        }
    }
    __printTextureInfo(Model);
    __loadTextures(Model);
}

void CGLTFObject::__printAttributes(const tinygltf::Primitive& vPrimitive) 
{
    HIVE_LOG_INFO("    Attributes:");
    for (const auto& Attr : vPrimitive.attributes) {
        HIVE_LOG_INFO("        {}", Attr.first);
    }
}

void CGLTFObject::__printTextureInfo(const tinygltf::Model& vModel) {
    // 检查模型是否包含纹理
    if (vModel.textures.empty()) {
        HIVE_LOG_WARNING("Model does not contain any textures.");
        return;
    }

    HIVE_LOG_INFO("Model contains the following textures:");

    for (size_t i = 0; i < vModel.textures.size(); ++i) {
        const tinygltf::Texture& Texture = vModel.textures[i];
        HIVE_LOG_INFO("Texture {} :", i);
        HIVE_LOG_INFO("  Name: ", Texture.name );

        // 检查纹理是否引用了图像
        if (Texture.source < 0 || Texture.source >= vModel.images.size()) {
            HIVE_LOG_INFO("  This Texture does not reference a valid image.");
        }
        else {
            const tinygltf::Image& image = vModel.images[Texture.source];
            HIVE_LOG_INFO("  Image width: {}", image.width);
            HIVE_LOG_INFO("  Image height: {}", image.height);
            HIVE_LOG_INFO("  Image format: {}", (image.component == 4 ? "RGBA" : "RGB"));
            HIVE_LOG_INFO("  Image data size: {} bytes", image.image.size());
        }
    }
}

// 加载模型中的所有纹理
void CGLTFObject::__loadTextures(const tinygltf::Model& vModel) {
    GLuint Index = 0;
    for (const auto& Texture : vModel.textures) {
        const auto& Img = vModel.images[Texture.source];
        auto Image = std::make_shared<CImage>();
        Image->loadTextureFromImage(Img);
        auto CurrentTexture = std::make_shared<CTexture>(*Image, Index);
        Index++;
        m_Images.push_back(Image);
        m_Textures.push_back(CurrentTexture);
    }
}

void CGLTFObject::renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight)
{
}
