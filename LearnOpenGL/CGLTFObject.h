#pragma once
#include "CRenderableObject.h"
#include <tiny_gltf.h>

class GLTRAINVER3_API CGLTFObject :
    public CRenderableObject
{
public:
    CGLTFObject() = default;
    void loadModelFromGLTF(const std::string& vPath);
private:
    void __printAttributes(const tinygltf::Primitive& vPrimitive);
};

