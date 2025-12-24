#include "vertexarrayobject.hpp"
#include "glad/glad.h"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
namespace graf
{
    
    VertexArrayObject::VertexArrayObject()
    {
        m_id =0;
        m_vbo = nullptr;
        m_vertexSize = 0.0f;
        
    }
    void VertexArrayObject::create()
    {
        glGenVertexArrays(1, &m_id); 
    }
    void VertexArrayObject::setVertexBuffer(VertexBuffer *vb)
    {
        m_vbo = vb;

        if(m_id!=0)
        {
            bind();
            m_vbo->bind();
        }
            
    }
    void VertexArrayObject::setIndexBuffer(IndexBuffer *ib)
    {
        m_ibo = ib;

        bind();
        m_ibo->bind();
    }
    void VertexArrayObject::draw()
    {
        bind();
        m_ibo->bind();
        glDrawElements(GL_TRIANGLES,m_ibo->indexCount(),GL_UNSIGNED_INT,0);
        m_ibo->unbind();
        unbind();
    }
    void VertexArrayObject::bind()
    {
         glBindVertexArray(m_id);

    }
    void VertexArrayObject::unbind()
    {
        glBindVertexArray(0);
    }
    void VertexArrayObject::addAttributes(VertexAttributeType newType)
    {
        m_attributes.emplace_back(newType);
        switch (newType)
        {
        case VertexAttributeType::Position:
            m_vertexSize+= 3*sizeof(float);
            break;
        case VertexAttributeType::Color:
            m_vertexSize+= 4*sizeof(float);
        case VertexAttributeType::Texture:
            m_vertexSize+= 2*sizeof(float);
        default:
            break;
        }
    }
    void VertexArrayObject::activateAttributes()
    {
        int totalAttributeSize=0;
        int activeAttributeSize= 0;
        for(int i=0;i<m_attributes.size();i++)
        {
            int floatCount = 0;

            switch (m_attributes[i])
            {
            case VertexAttributeType::Position:
                floatCount= 3;
                activeAttributeSize= sizeof(float)*3;
                break;
            case VertexAttributeType::Color:
                floatCount= 4;
                activeAttributeSize = sizeof(float)*3;
            case VertexAttributeType::Texture:
                floatCount= 2;
                activeAttributeSize = sizeof(float)*2;
            default:
                break;
            }
             glVertexAttribPointer(
                i, 
                floatCount, 
                GL_FLOAT, 
                GL_FALSE, 
                m_vertexSize, 
                (void*)totalAttributeSize);
            totalAttributeSize+=activeAttributeSize;
            glEnableVertexAttribArray(i);    

        }
    }
    VertexAttributeType toAttributeType(const std::string &strType)
    {
        VertexAttributeType type;
        if( strType=="Position")
            type=VertexAttributeType::Position;
        if(strType=="Normal") 
            type=VertexAttributeType::Normal;
        if(strType=="Color")
             type=VertexAttributeType::Color;
        if(strType=="Texture")
             type=VertexAttributeType::Texture;
                
                
        return type;
    }
}