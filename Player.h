#pragma once


#include <easing/Easing.h>
#include <Vector2.h>
#include "BaseObject.h"
#include "Collision/Collider.h"

#include <vector>
#include <chrono>
#include <memory>

class Player : public BaseObject
{
public:
    Player();
    ~Player();

    void Initialize();
    void RunSetMask();
    void Update();
    void Draw();
    void OnCollision(const Collider* _other);
    Collider* GetCollider() { return &collider_; }

private:
    char keys[256] = {};
    char preKeys[256] = {};

    Collider collider_ = {};

    std::vector<Vector2> vertices_ = {};

    float radius_default_ = 100.0f;
    float radius_min_ = 20.0f;
    float radius_current_ = 0.0f;
    float radius_timeRelease_ = 0.0f;

    size_t resolution_ = 4u;

    std::chrono::system_clock::time_point startTime_;
    std::unique_ptr<Easing> pEasingBoxResize_;
    std::unique_ptr<Easing> pEasingBoxTemp_;

private:
    CollisionManager* pCollisionManager_;
    void DebugWindow();
};