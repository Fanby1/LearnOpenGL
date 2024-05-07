#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CShader
{
public:
    unsigned int ID;
    CShader(const char* vVertexPath, const char* vFragmentPath);
    void use();
    void setBool(const std::string& vName, bool vValue) const;
    void setInt(const std::string& vName, int vValue) const;
    void setFloat(const std::string& vName, float vValue) const;
private:
    void checkCompileErrors(unsigned int vShader, std::string vType);
};

