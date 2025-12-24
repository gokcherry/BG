#pragma once
#include<string>
#include<vector>
#include <unordered_map>
#include<glm/glm.hpp>
using namespace std;
namespace graf
{
    class VertexArrayObject;

    class ShapeType
    {
    public:
        static string Circle;
        static string Quad;
        static string Cylinder;
        static string Cone;
        static string Pyramid;
    };

    class VertexUnlit
    {
    public:
        glm::vec3 position;
        glm::vec2 texCoords;
    };

    class MeshLoader
    {
    public:
        static VertexArrayObject* loadMeshFromFile(const string& fileName);
        static VertexArrayObject* createMesh(const string& shapeType);
    private:
        static int getVertexFloatCount(const vector<string>& attributes);
        static VertexArrayObject* createCircle();
        static VertexArrayObject* createCone();
        static VertexArrayObject* createCylinder();
        static VertexArrayObject* createPyramid();
        MeshLoader();
        static MeshLoader* m_instance;
        static MeshLoader* getInstance();
        unordered_map<string,VertexArrayObject*>  m_vaos;             
    }; 
}
