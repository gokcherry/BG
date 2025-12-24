#include "vertexbuffer.hpp"
#include "glad/glad.h"
namespace graf
{
    VertexBuffer::VertexBuffer()
    {
        m_id =0;
    }
    void VertexBuffer::create(void* data,unsigned int size)
    {
         glGenBuffers(1,&m_id);
         bind();
         glBufferData(GL_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);
    }
    void VertexBuffer::bind()
    {
         glBindBuffer(GL_ARRAY_BUFFER,m_id);
    }
    void VertexBuffer::unbind()
    {
         glBindBuffer(GL_ARRAY_BUFFER,0);
    }
}