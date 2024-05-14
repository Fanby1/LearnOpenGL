#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Eigen/Dense>

class CShader
{
public:
    unsigned int ID;
    Eigen::Matrix4f m_Transform = Eigen::Matrix4f::Identity();
    CShader(const char* vVertexPath, const char* vFragmentPath);
    void use();
    void setBool(const std::string& vName, bool vValue) const;
    void setInt(const std::string& vName, int vValue) const;
    void setFloat(const std::string& vName, float vValue) const;
    void setVec2(const std::string& name, const Eigen::Vector2f& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const Eigen::Vector3f& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const Eigen::Vector4f& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const Eigen::Matrix2f& mat) const;
    void setMat3(const std::string& name, const Eigen::Matrix3f& mat) const;
    void setMat4(const std::string& name, const Eigen::Matrix4f& mat) const;
private:
    void checkCompileErrors(unsigned int vShader, std::string vType);
};

