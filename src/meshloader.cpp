#define GLM_ENABLE_EXPERIMENTAL
#include "meshloader.hpp"
#include <fstream>
#include<iostream>
#include<nlohmann/json.hpp>
#include<glm/gtx/transform.hpp>
#include <vector>
#include "vertexbuffer.hpp"
#include "vertexarrayobject.hpp"
#include "indexbuffer.hpp"
namespace graf
{
    string ShapeType::Circle = "Circle";
    string ShapeType::Quad = "Quad";
    string ShapeType::Cylinder = "Cylinder";
    string ShapeType::Cone = "Cone";
    string ShapeType::Pyramid = "Pyramid";
    MeshLoader* MeshLoader::m_instance=nullptr;
    MeshLoader::MeshLoader()
    {
        
    }
    VertexArrayObject* MeshLoader::loadMeshFromFile(const string& fileName)
    {
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(fileName)==0)
        {

       
            string meshpath = "./meshes/"+fileName;
            ifstream file(meshpath);

            if(file.is_open())
            {
                cout<<"<------MeshLoader readling file "<<fileName<<"------->"<<endl;
                using namespace nlohmann;

                json data = json::parse(file);

                /*---------------------Parsing Mesh File--------------------------*/

                string meshName = data["MeshName"];
                int vertexAttributeCount = data["VertexAttributeCount"];
                vector<string> vertexAttributes = data["VertexAttributes"];
                int triangleCount = data["TriangleCount"];
                vector<float> vertices = data["Vertices"];
                vector<unsigned int> indices = data["Indices"];
                int vertexFloatCount = getVertexFloatCount(vertexAttributes);


                /*---------------------Creating Buffers--------------------------*/
                VertexBuffer* vbo = new VertexBuffer();
                vbo->create(&vertices[0],vertices.size()*sizeof(float));

                IndexBuffer* ibo = new IndexBuffer();
                ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


                vao = new VertexArrayObject();
                vao->create();
                vao->setVertexBuffer(vbo);
                vao->setIndexBuffer(ibo);
                for(auto nextAttribute:vertexAttributes)
                    vao->addAttributes(toAttributeType(nextAttribute));
                vao->activateAttributes();

                /*---------------------Printing Log Info--------------------------*/

                cout<<setw(20)<<std::left<<"MeshName"<<":"<<meshName<<endl;
                cout<<setw(20)<<std::left<<"VertexAttributeCount"<<":"<<vertexAttributeCount<<endl;
                cout<<setw(20)<<std::left<<"VertexAttributes"<<":";
                for(auto nextAttribute:vertexAttributes)
                    cout<<nextAttribute<<" ";
                cout<<endl;
                cout<<setw(20)<<std::left<<"TriangleCount"<<":"<<triangleCount<<endl;
                
                cout<<setw(30)<<"Vertices"<<":"<<endl;
                    
                for(int i=0;i<vertices.size();i+=vertexFloatCount)
                {
                    cout<<std::right<<setw(10)<<"Vertex"<<""<<setw(10)<<i/vertexFloatCount<<setw(5)<<" =>";
                    for(int j =0;j<vertexFloatCount;j++)
                    {
                        cout<<setw(6)<<std::right<<vertices[i+j];
                    }
                    cout<<endl;
                }           
            }
            else
            {
                cout<<"<------MeshLoader cannot open "<<fileName<<"------->"<<endl;
            }
        }
        else
        {
            vao = loader->m_vaos[fileName];
        }
        return vao;

    }
    VertexArrayObject *MeshLoader::createMesh(const string &shapeType)
    {
        if(shapeType==ShapeType::Circle)
            return createCircle();
        if(shapeType==ShapeType::Cone)
            return createCone();
        if(shapeType==ShapeType::Cylinder)
            return createCylinder();
        if(shapeType==ShapeType::Pyramid)
            return createPyramid();
        return nullptr;
    }
    VertexArrayObject* MeshLoader::createCone()
    {
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(ShapeType::Cone)==0)
        {
            vector<VertexUnlit> vertices;
            vector<unsigned int> indices;

            float angle =10;

            int vertexCount = 360.0f/angle;
            vertices.resize(vertexCount*2);
            for(int i=0;i<vertexCount;i++)
            {
                float nextAngle = i*angle;

                VertexUnlit vertex;
                vertex.position.x = cosf(glm::radians(nextAngle));
                vertex.position.y = 0;
                vertex.position.z = sinf(glm::radians(nextAngle));

                vertex.texCoords.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                vertex.texCoords.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));
                vertices[i] = vertex;

                vertices[i+vertexCount] = vertex;


            }
            float r = sqrtf(2);
            float totalAngle = 360.0f/r;
            angle = totalAngle/(vertexCount-1);


            VertexUnlit nextVertex;
            nextVertex.position = vertices[vertexCount].position;
            
            vertices.push_back(nextVertex);

            for(int i=0;i<=vertexCount;i++)
            {
                float nextAngle = angle*i;
                vertices[i+vertexCount].texCoords.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                vertices[i+vertexCount].texCoords.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));
           
            }


            VertexUnlit topVertex;
            topVertex.position = glm::vec3(0.0f,1.0f,0.0f);
            topVertex.texCoords = glm::vec2(0.5f,0.5f);
            vertices.push_back(topVertex);

            

            //----------------------------------------------------------------
            //---------------------Tabanın indeksleri bulunuyor---------------
            //----------------------------------------------------------------
            for(int i=0;i<vertexCount-2;i++)
            {
                indices.push_back(0);
                indices.push_back(i+1);
                indices.push_back(i+2);
            }
            //----------------------------------------------------------------
            //-----------------Yan yüzeyin indeksleri bulunuyor---------------
            //----------------------------------------------------------------   
            
            int topIndex = vertices.size()-1;
            for(int i=0;i<vertexCount-1;i++)
            {
                indices.push_back(topIndex);
                indices.push_back(vertexCount+i);
                indices.push_back(vertexCount+i+1);
            }
            indices.push_back(topIndex);
            indices.push_back(topIndex-1);
            indices.push_back(topIndex-2);

            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();           

            loader->m_vaos[ShapeType::Cone] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Cone];
        }
        return vao;        
    }
    VertexArrayObject* MeshLoader::createCylinder()
    {
        VertexArrayObject* vao=nullptr;
        auto loader = getInstance();
        if(loader->m_vaos.count(ShapeType::Cylinder)==0)
        {
            std::vector<VertexUnlit> vertices;
            std::vector<unsigned int> indices;
            float angleStep = 10.0f;
            int segmentCount = static_cast<int>(360.0f/angleStep);
            //side vertices
            for(int i=0;i<segmentCount;i++)
            {
                float rad = glm::radians(i*angleStep);
                float x = cosf(rad);
                float z = sinf(rad);
                float texS = static_cast<float>(i)/segmentCount;
                VertexUnlit bottom;
                bottom.position = glm::vec3(x,-1.0f,z);
                bottom.texCoords = glm::vec2(texS,0.0f);
                VertexUnlit top;
                top.position = glm::vec3(x,1.0f,z);
                top.texCoords = glm::vec2(texS,1.0f);
                vertices.push_back(bottom);
                vertices.push_back(top);
            }
            //side indices
            for(int i=0;i<segmentCount;i++)
            {
                int next = (i+1)%segmentCount;
                unsigned int b0 = i*2;
                unsigned int t0 = i*2+1;
                unsigned int b1 = next*2;
                unsigned int t1 = next*2+1;
                indices.push_back(b0); indices.push_back(b1); indices.push_back(t1);
                indices.push_back(b0); indices.push_back(t1); indices.push_back(t0);
            }
            //top and bottom caps
            unsigned int topCenterIndex = vertices.size();
            VertexUnlit topCenter;
            topCenter.position = glm::vec3(0.0f,1.0f,0.0f);
            topCenter.texCoords = glm::vec2(0.5f,0.5f);
            vertices.push_back(topCenter);

            unsigned int bottomCenterIndex = vertices.size();
            VertexUnlit bottomCenter;
            bottomCenter.position = glm::vec3(0.0f,-1.0f,0.0f);
            bottomCenter.texCoords = glm::vec2(0.5f,0.5f);
            vertices.push_back(bottomCenter);

            for(int i=0;i<segmentCount;i++)
            {
                int next = (i+1)%segmentCount;
                unsigned int t0 = i*2+1;
                unsigned int t1 = next*2+1;
                unsigned int b0 = i*2;
                unsigned int b1 = next*2;

                indices.push_back(topCenterIndex);
                indices.push_back(t0);
                indices.push_back(t1);

                indices.push_back(bottomCenterIndex);
                indices.push_back(b1);
                indices.push_back(b0);
            }

            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();           

            loader->m_vaos[ShapeType::Cylinder] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Cylinder];
        }
        return vao;        
    }
    VertexArrayObject* MeshLoader::createPyramid()
    {
        VertexArrayObject* vao=nullptr;
        auto loader = getInstance();
        if(loader->m_vaos.count(ShapeType::Pyramid)==0)
        {
            std::vector<VertexUnlit> vertices;
            std::vector<unsigned int> indices;

            //base
            VertexUnlit v0; v0.position = glm::vec3(-1.0f,-1.0f,-1.0f); v0.texCoords = glm::vec2(0.0f,0.0f);
            VertexUnlit v1; v1.position = glm::vec3(1.0f,-1.0f,-1.0f);  v1.texCoords = glm::vec2(1.0f,0.0f);
            VertexUnlit v2; v2.position = glm::vec3(1.0f,-1.0f,1.0f);   v2.texCoords = glm::vec2(1.0f,1.0f);
            VertexUnlit v3; v3.position = glm::vec3(-1.0f,-1.0f,1.0f);  v3.texCoords = glm::vec2(0.0f,1.0f);
            VertexUnlit apex; apex.position = glm::vec3(0.0f,1.0f,0.0f); apex.texCoords = glm::vec2(0.5f,0.0f);

            //side faces (duplicate vertices for texture)
            VertexUnlit s0 = v0; s0.texCoords = glm::vec2(0.0f,1.0f);
            VertexUnlit s1 = v1; s1.texCoords = glm::vec2(1.0f,1.0f);
            VertexUnlit s2 = v2; s2.texCoords = glm::vec2(1.0f,1.0f);
            VertexUnlit s3 = v3; s3.texCoords = glm::vec2(0.0f,1.0f);
            VertexUnlit a0 = apex; a0.texCoords = glm::vec2(0.5f,0.0f);
            VertexUnlit a1 = apex; a1.texCoords = glm::vec2(0.5f,0.0f);
            VertexUnlit a2 = apex; a2.texCoords = glm::vec2(0.5f,0.0f);
            VertexUnlit a3 = apex; a3.texCoords = glm::vec2(0.5f,0.0f);

            //push base
            unsigned int baseStart = vertices.size();
            vertices.push_back(v0); vertices.push_back(v1); vertices.push_back(v2); vertices.push_back(v3);
            //side sets
            unsigned int sideStart = vertices.size();
            vertices.push_back(s0); vertices.push_back(s1); vertices.push_back(a0); //front
            vertices.push_back(s1); vertices.push_back(s2); vertices.push_back(a1); //right
            vertices.push_back(s2); vertices.push_back(s3); vertices.push_back(a2); //back
            vertices.push_back(s3); vertices.push_back(s0); vertices.push_back(a3); //left

            //indices
            indices = {
                baseStart, baseStart+1, baseStart+2,
                baseStart, baseStart+2, baseStart+3
            };
            for(int face=0; face<4; ++face)
            {
                unsigned int idx = sideStart + face*3;
                indices.push_back(idx);
                indices.push_back(idx+1);
                indices.push_back(idx+2);
            }

            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();           

            loader->m_vaos[ShapeType::Pyramid] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Pyramid];
        }
        return vao;        
    }
    int MeshLoader::getVertexFloatCount(const vector<string> &attributes)
    {
        int floatCount =0;

        for(auto nextAttribute:attributes)
        {
            if( nextAttribute=="Position"||
                nextAttribute=="Normal"
                )
                floatCount+=3;
            if(nextAttribute=="Color")
                floatCount+=4;       
            if(nextAttribute=="Texture")
                floatCount+=2;            
        }

        return floatCount;
    }

    VertexArrayObject *MeshLoader::createCircle()
    {
        VertexArrayObject* vao=nullptr;

        auto loader = getInstance();

        if(loader->m_vaos.count(ShapeType::Circle)==0)
        {
            vector<VertexUnlit> vertices;
            vector<unsigned int> indices;

            float angle =10;

            int vertexCount = 360.0f/angle;

            for(int i=0;i<vertexCount;i++)
            {
                float nextAngle = i*angle;

                VertexUnlit vertex;
                vertex.position.x = cosf(glm::radians(nextAngle));
                vertex.position.y = sinf(glm::radians(nextAngle));
                vertex.position.z = 0.0f;

                vertex.texCoords.s = 0.5f+0.5f*cosf(glm::radians(nextAngle));
                vertex.texCoords.t = 0.5f+0.5f*sinf(glm::radians(nextAngle));
                vertices.push_back(vertex);
            }


            for(int i=0;i<vertexCount-2;i++)
            {
                indices.push_back(0);
                indices.push_back(i+1);
                indices.push_back(i+2);
            }


            VertexBuffer* vbo = new VertexBuffer();
            vbo->create(&vertices[0],vertices.size()*sizeof(VertexUnlit));

            IndexBuffer* ibo = new IndexBuffer();
            ibo->create(indices.data(),indices.size()*sizeof(unsigned int));


            vao = new VertexArrayObject();
            vao->create();
            vao->setVertexBuffer(vbo);
            vao->setIndexBuffer(ibo);
            vao->addAttributes(VertexAttributeType::Position);
            vao->addAttributes(VertexAttributeType::Texture);
            vao->activateAttributes();           

            loader->m_vaos[ShapeType::Circle] = vao;
        }
        else
        {
            vao = loader->m_vaos[ShapeType::Circle];
        }
        return vao;

    }

   MeshLoader *MeshLoader::getInstance()
    {
        if(!m_instance)
            m_instance = new MeshLoader;
        return m_instance;
    }
}
