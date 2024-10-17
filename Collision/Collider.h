#pragma once

#include "Shape.h"
#include "Vector2.h"
#include <vector>
#include <string>
#include <functional>

class CollisionManager;

class Collider
{
public:

    inline  uint32_t                    GetCollisionAttribute()     const       { return collisionAttribute_; }
    inline  uint32_t                    GetCollisionMask()          const       { return *pCollisionMask_;}
    inline  Shape                       GetShape()                  const       { return shape_; }
    inline  unsigned int                GetRadius()                 const       { return radiusCollider_; }
    inline  std::vector<Vector2>*       GetVertices()                           { return &verticesCollider_; }
    inline  const std::string&          GetColliderID()             const       { return colliderID_; }

    inline  void                        SetColliderID(const std::string& _id)   { colliderID_ = _id; }
    void                                SetVertices(const std::vector<Vector2>&& _vertices);
    void                                SetVertices(const std::vector<Vector2>* _vertices);
    void                                SetVertices(Vector2* _vertices, size_t _size);
    void                                SetAttribute(uint32_t _attribute);
    void                                SetMask(uint32_t* _mask);
    void                                SetOnCollision(const std::function<void(const Collider*)>& _func) { onCollisionFunction_ = _func; }

    inline  void                        OnCollision(const Collider* _other)
    {
        if (onCollisionFunction_)
            onCollisionFunction_(_other);
        return;
    }

private:

    std::function<void(const Collider*)> onCollisionFunction_;

    Shape                   shape_              = Shape::Polygon;
    std::string             colliderID_         = {};

    std::vector<Vector2>    verticesCollider_   = {};
    unsigned int            radiusCollider_     = 0u;

    // 衝突属性(自分)
    uint32_t                collisionAttribute_ = 0xffffffff;
    // 衝突マスク(相手)
    uint32_t*               pCollisionMask_     = new uint32_t(0xffffffff);

};