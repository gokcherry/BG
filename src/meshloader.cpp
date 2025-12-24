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