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
    HIVE_LOG_INFO("Number of meshes: ", Model.meshes.size());
    for (size_t i = 0; i < Model.meshes.size(); ++i) {
        const tinygltf::Mesh& Mesh = Model.meshes[i];
        HIVE_LOG_INFO("Mesh {} name: {}", i, Mesh.name);
        for (size_t j = 0; j < Mesh.primitives.size(); ++j) {
            const tinygltf::Primitive& Primitive = Mesh.primitives[j];
            HIVE_LOG_INFO("    Primitive {}", j);
            __printAttributes(Primitive);
        }
    }
}

void CGLTFObject::__printAttributes(const tinygltf::Primitive& vPrimitive) 
{
    HIVE_LOG_INFO("    Attributes:");
    for (const auto& Attr : vPrimitive.attributes) {
        HIVE_LOG_INFO("        {}", Attr.first);
    }
}

void CGLTFObject::renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight)
{
}
