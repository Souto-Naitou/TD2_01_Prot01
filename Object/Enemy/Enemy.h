#pragma once

#include "BaseObject.h"
#include "Collision/Collider.h"
#include <Vector2.h>

class Enemy : public BaseObject
{
public:

    Enemy() = default;
    ~Enemy();

    void Initialize(size_t idx);
    void Update();
    void Draw();

    Collider* GetCollider() { return &collider_; }

    void SetTargetPosition(const Vector2& _target) { positionTarget_ = _target; }
    void OnCollision(const Collider* _other);

private:
    Collider collider_ = {};
    Vector2 vertices_[3] = {};

    std::string idx_;

    Vector2 positionTarget_ = {};
    Vector2 distanceToTarget = {};
    Vector2 ellipseAB_ = {};
    float   rotation_       = 0.0f;
    float radius_ = 0.0f;
    float moveSpeed_ = 0.0f;
    float collisionCooldown_ = 0.0f;
    const float collisionCooldownDuration_ = 0.5f;

    char* keys_ = nullptr;
    char* preKeys_ = nullptr;

    // 衝突時のぶっとび速度
    float bounceSpeed_ = 5.0f;
    bool isBouncing_ = false;
    //衝突フラグ
    bool hasCollided_ = false;

    void DebugWindow();
};