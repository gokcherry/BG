#include "sceneobject.hpp"
#include "transform.hpp"
#include<algorithm>
namespace graf
{
    SceneObject::SceneObject()
    {
        transform= new Transform();
        m_name = "Empty";
        m_parent = nullptr;
        m_type = SceneObjectType::Empty;

    }
    void SceneObject::addChild(SceneObject *child)
    {
        m_childs.push_back(child);
        child->m_parent = this;
    }
    void SceneObject::removeChild(SceneObject *child)
    {
        auto it= std::find(m_childs.begin(),m_childs.end(),child);
        if(it!=m_childs.end())
        {
            m_childs.erase(it);
            child->m_parent = nullptr;
        }
    }
    SceneObjectType SceneObject::getType() const
    {
        return m_type;
    }
    string SceneObject::getName() const
    {
        return m_name;
    }
    SceneObject* SceneObject::getParent() const
    {
        return m_parent;
    }
    void SceneObject::updateChilds()
    {
        auto combinedMatrix=transform->parentCombinedMatrix*transform->worldMatrix;

        for(auto nextChild:m_childs)
        {
            nextChild->transform->parentCombinedMatrix = combinedMatrix;
            nextChild->updateChilds();
        }
    }
}
