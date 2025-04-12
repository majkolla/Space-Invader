#include "gameobject.hpp"
#include <cmath>


GameObject::GameObject()
    : m_removed{false}
{
}

bool GameObject::collides(const GameObject *other) const
{
    return bounds().intersects(other->bounds());
}

void GameObject::remove()
{
    m_removed = true;
}

bool GameObject::is_removed() const
{
    return m_removed;
}
