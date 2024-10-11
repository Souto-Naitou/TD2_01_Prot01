#pragma once

#include "BaseObject.h"
#include <Vector2.h>

class Enemy : public BaseObject
{
public:

    Enemy() = default;
    ~Enemy() = default;

    void Initialize();
    void Update();
    void Draw();

    void SetTargetPosition(const Vector2& _target) { positionTarget_ = _target; }

private:
    Vector2 vertices[3] = {};

    Vector2 positionTarget_ = {};
    Vector2 distanceToTarget = {};
    Vector2 ellipseAB_ = {};
    float   rotation_       = 0.0f;
    float radius_ = 0.0f;

    char* keys_ = nullptr;
    char* preKeys_ = nullptr;

    void DebugWindow();
};