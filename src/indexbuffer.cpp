#include "indexbuffer.hpp"

#include<glad/glad.h>
namespace graf
{

    void IndexBuffer::create(void * data,int size)
    {

        //Index buffer object oluşuruluyor
        glGenBuffers(1,&m_id);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_id);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);

        m_indexCount = size/4;

    }
    int IndexBuffer::indexCount()const
    {
        return m_indexCount;
    }
    void IndexBuffer::bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_id);
    }
    void IndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }    
    void IndexBuffer::release()
    {
        glDeleteBuffers(1,&m_id);
    }
}