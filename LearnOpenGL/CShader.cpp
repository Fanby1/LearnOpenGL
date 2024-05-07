#include "CShader.h"

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
