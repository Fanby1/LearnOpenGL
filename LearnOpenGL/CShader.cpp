#include "CShader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "HiveLogger.h"

void CShader::__computeTransformMatrix()
{
    m_Transform = m_Projection * m_View * m_Model;
    m_ComputeTransformFlag = false;
}

CShader::CShader(const char* vVertexPath, const char* vFragmentPath)
{
    // 1. retrieve the Vertex/Fragment source code from filePath
    std::string VertexCode;
    std::string FragmentCode;
    std::ifstream VertexShaderFile;
    std::ifstream FragmentShaderFile;
    // ensure ifstream objects can throw exceptions:
    VertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    FragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        VertexShaderFile.open(vVertexPath);
        FragmentShaderFile.open(vFragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << VertexShaderFile.rdbuf();
        fShaderStream << FragmentShaderFile.rdbuf();
        // close file handlers
        VertexShaderFile.close();
        FragmentShaderFile.close();
        // convert stream into string
        VertexCode = vShaderStream.str();
        FragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        HIVE_LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", e.what());
    }
    const char* VertexShaderCode = VertexCode.c_str();
    const char* FragmentShaderCode = FragmentCode.c_str();
    unsigned int Vertex, Fragment;

    Vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vertex, 1, &VertexShaderCode, NULL);
    glCompileShader(Vertex);
    __checkCompileErrors(Vertex, "VERTEX");

    Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fragment, 1, &FragmentShaderCode, NULL);
    glCompileShader(Fragment);
    __checkCompileErrors(Fragment, "FRAGMENT");

    m_ProjectID = glCreateProgram();
    glAttachShader(m_ProjectID, Vertex);
    glAttachShader(m_ProjectID, Fragment);
    glLinkProgram(m_ProjectID);
    __checkCompileErrors(m_ProjectID, "PROGRAM");

    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
}

unsigned int CShader::getID() {
    return m_ProjectID;
}

// º¯Êý¼ì²é OpenGL ´íÎó
void checkOpenGLError(const char* stmt, const char* fname, int line);

#define CHECK_GL_ERROR(stmt) do { \
    stmt; \
    checkOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)


void CShader::use()
{
    CHECK_GL_ERROR(glUseProgram(m_ProjectID));
    if (m_ComputeTransformFlag) {
        __computeTransformMatrix();
    }
    setMat4("model", m_Model);
    setMat4("view", m_View);
    setMat4("projection", m_Projection);
    setMat4("transform", m_Transform);
}

void CShader::setModel(Eigen::Matrix4f vModelMatrix)
{
    m_Model = vModelMatrix;
    m_ComputeTransformFlag = true;
}

void CShader::setView(Eigen::Matrix4f vVeiwMatrix)
{
    m_View = vVeiwMatrix;
    m_ComputeTransformFlag = true;
}

void CShader::setProjection(Eigen::Matrix4f vProjectionMatrix)
{
    m_Projection = vProjectionMatrix;
    m_ComputeTransformFlag = true;
}

template<typename T>
void CShader::setUniform(const std::string& vName, T vValue) const {
    GLint VarLocation = glGetUniformLocation(m_ProjectID, vName.c_str());
}

void CShader::setBool(const std::string& vName, bool vValue) const
{
    glUniform1i(glGetUniformLocation(m_ProjectID, vName.c_str()), (int)vValue);
}

void CShader::setInt(const std::string& vName, int vValue) const
{
    glUniform1i(glGetUniformLocation(m_ProjectID, vName.c_str()), vValue);
}

void CShader::setFloat(const std::string& vName, float vValue) const
{
    glUniform1f(glGetUniformLocation(m_ProjectID, vName.c_str()), vValue);
}

// ------------------------------------------------------------------------
void CShader::setVec2(const std::string& name, const Eigen::Vector2f& value) const
{
    glUniform2fv(glGetUniformLocation(m_ProjectID, name.c_str()), 1, value.data());
}
void CShader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_ProjectID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void CShader::setVec3(const std::string& name, const Eigen::Vector3f& value) const
{
    glUniform3fv(glGetUniformLocation(m_ProjectID, name.c_str()), 1, value.data());
}
void CShader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_ProjectID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void CShader::setVec4(const std::string& name, const Eigen::Vector4f& value) const
{
    glUniform4fv(glGetUniformLocation(m_ProjectID, name.c_str()), 1, value.data());
}
void CShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(m_ProjectID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void CShader::setMat2(const std::string& name, const Eigen::Matrix2f& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_ProjectID, name.c_str()), 1, GL_FALSE, mat.data());
}
// ------------------------------------------------------------------------
void CShader::setMat3(const std::string& name, const Eigen::Matrix3f& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_ProjectID, name.c_str()), 1, GL_FALSE, mat.data());
}
// ------------------------------------------------------------------------
void CShader::setMat4(const std::string& name, const Eigen::Matrix4f& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ProjectID, name.c_str()), 1, GL_FALSE, mat.data());
}

void CShader::__checkCompileErrors(unsigned int vShader, std::string vType)
{
    int Success;
    char InfoLog[1024];
    if (vType != "PROGRAM")
    {
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            glGetShaderInfoLog(vShader, 1024, NULL, InfoLog);
            HIVE_LOG_ERROR("ERROR::SHADER_COMPILATION_ERROR of vType: {}\n{}", vType, InfoLog);
        }
    }
    else
    {
        glGetProgramiv(vShader, GL_LINK_STATUS, &Success);
        if (!Success)
        {
            glGetProgramInfoLog(vShader, 1024, NULL, InfoLog);
            HIVE_LOG_ERROR("ERROR::PROGRAM_LINKING_ERROR of vType: {}\n{}", vType, InfoLog);
        }
    }
}
