#pragma once
#include<vector>
#include<string>
namespace graf
{
    enum class VertexAttributeType
    {
        Position,
        Color,
        Texture,
        Normal
    };

    VertexAttributeType toAttributeType(const std::string& strType);
    class VertexBuffer;
    class IndexBuffer;
    class VertexArrayObject
    {
    public:
        VertexArrayObject();
        void create();
        void setVertexBuffer(VertexBuffer* vb);
        void setIndexBuffer(IndexBuffer* ib);
        void draw();
        void bind();
        void unbind();
        void addAttributes(VertexAttributeType newType);
        void activateAttributes();
    private:
        unsigned int m_id;
        VertexBuffer* m_vbo;
        IndexBuffer* m_ibo;
        unsigned int m_vertexSize;
        std::vector<VertexAttributeType> m_attributes;
    };
}