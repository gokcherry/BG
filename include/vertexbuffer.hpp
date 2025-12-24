#pragma once

namespace graf
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        void create(void * data,unsigned int size);
        void bind();
        void unbind();
    private:

        unsigned int m_id;
    };

}