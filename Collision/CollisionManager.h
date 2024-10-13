#pragma once

#include "Collider.h"
#include <vector>
#include <string>
#include <utility>

class CollisionManager
{
public:
    void Initialize();
    void CheckAllCollision();
    void RegisterCollider(Collider* _collider);
    void ClearColliderList();
    int32_t GetNewAttribute(std::string _id);

private:
    std::vector<Collider*> colliders_;
    std::vector<std::pair<std::string, std::string>> collisionNames_;
    std::vector<std::pair<std::string, int32_t>> attributeList_;

    void DebugWindow();
    void CheckCollisionPair(Collider* _colA, Collider* _colB);
    void ProjectShapeOnAxis(std::vector<Vector2>* _v, const Vector2& _axis, float& _min, float& _max);
};