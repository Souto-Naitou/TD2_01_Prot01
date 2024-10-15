#include "Collider.h"
#include "CollisionManager.h"

void Collider::SetVertices(const std::vector<Vector2>&& _vertices)
{
    verticesCollider_ = _vertices;
}

void Collider::SetVertices(const std::vector<Vector2>* _vertices)
{
    verticesCollider_ = *_vertices;
}

void Collider::SetVertices(Vector2* _vertices, size_t _size)
{
    verticesCollider_.clear();
    for (int i = 0; i < _size; i++)
    {
        verticesCollider_.push_back(*(_vertices + i));
    }
}

void Collider::SetAttribute(uint32_t _attribute)
{
    collisionAttribute_ = _attribute;
}

void Collider::SetMask(uint32_t* _mask)
{
    pCollisionMask_ = _mask;
}
