#include "CShaderFrame.h"

unsigned int CShaderFrame::generateShader(const char* vSource, unsigned int Type)
{
    unsigned int Shader = glCreateShader(Type);
    glShaderSource(Shader, 1, &vSource, NULL);
    glCompileShader(Shader);
    // check for shader compile errors
    int Success;
    char InfoLog[512];
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
    if (Success) {
        return Shader;
    }
    switch (Type)
    {
    case GL_VERTEX_SHADER:
        glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << std::endl;
    case GL_FRAGMENT_SHADER:
        glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << std::endl;
    }
    return -1;
}
