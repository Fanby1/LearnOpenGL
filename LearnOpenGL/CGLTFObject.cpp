#include "CGLTFObject.h"
#include <HiveLogger.h>
#include <Singleton.h>
#include <tiny_gltf.h>
#include <vector>
#include "CVertexBufferObject.h";
#include "CVertexArrayObject.h"


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
            HIVE_LOG_INFO("  Primitive {}", j);
            __printAndLoadAttributes(Model, Primitive);
        }
    }
    __printTextureInfo(Model);
    __loadTextures(Model);
}

void CGLTFObject::__printAndLoadAttributes(const tinygltf::Model& vModel, const tinygltf::Primitive& vPrimitive)
{
    HIVE_LOG_INFO("  Attributes:");
    unsigned long TotalVertex = 0;
    unsigned long TotalVertexLength = 0;
    std::vector<float> GLFormatData;
    for (const auto& Attr : vPrimitive.attributes) 
    {
        int AccessorIndex = Attr.second;
        const tinygltf::Accessor& Accessor = vModel.accessors[AccessorIndex];
        const tinygltf::BufferView& BufferView = vModel.bufferViews[Accessor.bufferView];
        const tinygltf::Buffer& Buffer = vModel.buffers[BufferView.buffer];

        HIVE_LOG_INFO("    {}", Attr.first);
        HIVE_LOG_INFO("    Accessor: {}", AccessorIndex);
        HIVE_LOG_INFO("    ByteOffset: {}",Accessor.byteOffset);
        HIVE_LOG_INFO("    Count: {}", Accessor.count);
        HIVE_LOG_INFO("    Type: {}", Accessor.type);
        __printBufferView(vModel, Accessor.bufferView);
        TotalVertex = Accessor.count;
        TotalVertexLength += Accessor.type;
    }
    GLFormatData.resize(TotalVertex * TotalVertexLength);
    for (const auto& Attr : vPrimitive.attributes)
    {
        int AccessorIndex = Attr.second;
        const tinygltf::Accessor& Accessor = vModel.accessors[AccessorIndex];
        const tinygltf::BufferView& BufferView = vModel.bufferViews[Accessor.bufferView];
        const tinygltf::Buffer& Buffer = vModel.buffers[BufferView.buffer];
        auto DataBegin = __getDataPointer(vModel, Accessor);
        int Offset = 0;
        if (Attr.first == "NORMAL") 
        {
            Offset = 5;
        }
        else if (Attr.first == "TEXCOORD_0") 
        {
            Offset = 3;
        }
        __rearrangeArray(DataBegin, Accessor.count * Accessor.type, Accessor.type, GLFormatData, TotalVertexLength, Offset);
    }
    std::vector<unsigned int> Offset = { 3,0,2,3 };
    auto VBO = std::make_shared<CVertexBufferObject>(GLFormatData.data(), GLFormatData.size() * sizeof(float), VERTEX_TYPE_VERTEX_BIT | VERTEX_TYPE_TEXTURE_BIT | VERTEX_TYPE_NORMAL_BIT, Offset);
    auto VAO = std::make_shared<CVertexArrayObject>();
    VAO->addVBO(VBO);
    addVAO(VAO, m_Shader);
}


const float* CGLTFObject::__getDataPointer(const tinygltf::Model& vModel, const tinygltf::Accessor& vAccessor) {
    const tinygltf::BufferView& BufferView = vModel.bufferViews[vAccessor.bufferView];
    const tinygltf::Buffer& Buffer = vModel.buffers[BufferView.buffer];
    auto Point = Buffer.data.data() + BufferView.byteOffset + vAccessor.byteOffset;
    return reinterpret_cast<const float*>(Point);
}

void CGLTFObject::__printBufferView(const tinygltf::Model& vModel, int vBufferViewIndex) {
    const tinygltf::BufferView& BufferView = vModel.bufferViews[vBufferViewIndex];
    const tinygltf::Buffer& Buffer = vModel.buffers[BufferView.buffer];

    HIVE_LOG_INFO("      BufferView: {}", vBufferViewIndex);
    HIVE_LOG_INFO("        ByteOffset: {}" ,BufferView.byteOffset);
    HIVE_LOG_INFO("        ByteLength: {}", BufferView.byteLength);
    HIVE_LOG_INFO("        Buffer Data Size: {} bytes", Buffer.data.size());
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
    if (__isFunctionSet()) {
        auto Current = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> Elapsed = Current - m_Start;
        m_UpdateMoveFunction(Elapsed, *this);
    }
    Eigen::Vector3f LightPosition = { 0,0,0 };
    if (vLight) {
        Eigen::Vector3f LightPosition = vLight->getPosition();
    }
    for (auto& It : m_VAOs) {
        __transform(It.second);
        It.second->use();
        if (vCamera) {
            vCamera->updateShaderUniforms(It.second);
        }
        It.second->setVec3("viewPos", vCamera->getPosition());
        if (vLight) {
            It.second->setVec3("lightPos", LightPosition);
            It.second->setVec3("lightColor", { 1,1,1 });
        }
        if (vDirectionalLight) {
            vDirectionalLight->updateShaderUniforms(It.second);
        }

        It.first->bind();
        if (It.first->getEBO() != nullptr) {
            glDrawElements(GL_TRIANGLES, It.first->getEBO()->getSize(), GL_UNSIGNED_INT, 0);
        }
        else {
            auto VBOs = It.first->getVBOs();
            auto VBO = VBOs.begin();
            auto size = VBO->get()->getSize();
            glDrawArrays(GL_TRIANGLES, 0, VBO->get()->getSize());
        }
        glBindVertexArray(0);
    }
}
