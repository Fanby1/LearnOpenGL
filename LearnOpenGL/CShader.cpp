#include "CShader.h"

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
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* VertexShaderCode = VertexCode.c_str();
    const char* FragmentShaderCode = FragmentCode.c_str();
    // 2. compile shaders
    unsigned int Vertex, Fragment;
    // Vertex shader
    Vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(Vertex, 1, &VertexShaderCode, NULL);
    glCompileShader(Vertex);
    checkCompileErrors(Vertex, "VERTEX");
    // Fragment CShader
    Fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(Fragment, 1, &FragmentShaderCode, NULL);
    glCompileShader(Fragment);
    checkCompileErrors(Fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, Vertex);
    glAttachShader(ID, Fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
}

void CShader::use()
{
    glUseProgram(ID);
    if (m_ComputeTransformFlag) {
        __computeTransformMatrix();
    }
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

void CShader::setBool(const std::string& vName, bool vValue) const
{
    glUniform1i(glGetUniformLocation(ID, vName.c_str()), (int)vValue);
}

void CShader::setInt(const std::string& vName, int vValue) const
{
    glUniform1i(glGetUniformLocation(ID, vName.c_str()), vValue);
}

void CShader::setFloat(const std::string& vName, float vValue) const
{
    glUniform1f(glGetUniformLocation(ID, vName.c_str()), vValue);
}

// ------------------------------------------------------------------------
void CShader::setVec2(const std::string& name, const Eigen::Vector2f& value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, value.data());
}
void CShader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void CShader::setVec3(const std::string& name, const Eigen::Vector3f& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value.data());
}
void CShader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void CShader::setVec4(const std::string& name, const Eigen::Vector4f& value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value.data());
}
void CShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void CShader::setMat2(const std::string& name, const Eigen::Matrix2f& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
}
// ------------------------------------------------------------------------
void CShader::setMat3(const std::string& name, const Eigen::Matrix3f& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
}
// ------------------------------------------------------------------------
void CShader::setMat4(const std::string& name, const Eigen::Matrix4f& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
}

void CShader::checkCompileErrors(unsigned int vShader, std::string vType)
{
    int Success;
    char InfoLog[1024];
    if (vType != "PROGRAM")
    {
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            glGetShaderInfoLog(vShader, 1024, NULL, InfoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of vType: " << vType << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(vShader, GL_LINK_STATUS, &Success);
        if (!Success)
        {
            glGetProgramInfoLog(vShader, 1024, NULL, InfoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of vType: " << vType << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
