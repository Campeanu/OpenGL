#ifndef __INDEX_HPP_INCLUDED__
#define __INDEX_HPP_INCLUDED__

class IndexBuffer {
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    virtual ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }

private:
    unsigned int m_RendererID;
    unsigned int m_Count;
};

#endif // !__INDEX_HPP_INCLUDED__
