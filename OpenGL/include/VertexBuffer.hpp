#ifndef __VERTEXBUFFER_HPP_INCLUDED__
#define __VERTEXBUFFER_HPP_INCLUDED__

class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size);
    virtual ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_RendererID;
};

#endif // !__VERTEXBUFFER_HPP_INCLUDED__
