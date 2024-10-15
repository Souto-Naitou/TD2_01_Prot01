#pragma once

#include "BaseObject.h"
#include "Collision/Collider.h"
#include <Vector2.h>

class Enemy : public BaseObject
{
public:

    Enemy() = default;
    ~Enemy() = default;

    void Initialize();
    void Update();
    void Draw();

    Collider* GetCollider() { return &collider_; }

    void SetTargetPosition(const Vector2& _target) { positionTarget_ = _target; }
    void OnCollision(const Collider* _other);
private:
    Collider collider_ = {};
    Vector2 vertices_[3] = {};

    Vector2 positionTarget_ = {};
    Vector2 distanceToTarget = {};
    Vector2 ellipseAB_ = {};
    float   rotation_       = 0.0f;
    float radius_ = 0.0f;

    char* keys_ = nullptr;
    char* preKeys_ = nullptr;

    void DebugWindow();
};