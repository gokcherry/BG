#pragma once
namespace graf
{
    class IndexBuffer
    {
    public:
        void create(void* data,int size);
        void bind();
        void unbind();
        void release();
        int  indexCount()const;
    private:
        unsigned int m_indexCount;
        unsigned int m_id;
    };    
}