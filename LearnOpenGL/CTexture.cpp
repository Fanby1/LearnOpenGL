#include "CTexture.h"

CTexture::CTexture(const CImage& vImage, GLuint vTextureUnit)
{
    m_TextureUnit = vTextureUnit;
    glActiveTexture(vTextureUnit);
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    unsigned long RGBA = GL_RGB;
    if (vImage.getNRChannels() == 4) {
        RGBA = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, RGBA, vImage.getWidth(), vImage.getHeight(), 0, RGBA, GL_UNSIGNED_BYTE, vImage.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void CTexture::bind()
{
    glActiveTexture(m_TextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

GLuint CTexture::getTexture() const
{
    return m_Texture;
}
