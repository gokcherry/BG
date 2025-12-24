#pragma once
#include<string>
#include <vector>
using namespace std;
namespace graf
{

    enum class SceneObjectType
    {
        Empty,
        Model,
        Camera,
        Light
    };
    class SceneObject;
    using SceneObjectList = std::vector<SceneObject*>;

    class Transform;
    class SceneObject
    {
    public:
        SceneObject();
        Transform* transform;
        void addChild(SceneObject* child);
        void removeChild(SceneObject* child);
        SceneObjectType getType() const;
        string getName()const;
        SceneObject* getParent() const;
        void updateChilds();
    protected:
        friend class Scene;
        SceneObjectList m_childs;
        SceneObjectType m_type;
        string m_name;
        SceneObject* m_parent;

    };
}
