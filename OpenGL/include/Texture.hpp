#ifndef __TEXTURE_HPP_INCLUDED__
#define __TEXTURE_HPP_INCLUDED__

#include "Renderer.hpp"

class Texture {
public:
    Texture(const std::string& path);
    virtual ~Texture();
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
};

#endif // !__TEXTURE_HPP_INCLUDED__