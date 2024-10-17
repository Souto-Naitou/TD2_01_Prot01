#pragma once

#include <BaseObject.h>
#include <Rect2.h>
#include "Collision/Collider.h"

class Core : public BaseObject
{
public:

    Core();
    ~Core();

    void Initialize();
    void RunSetMask();
    void Update();
    void Draw();

    // コールバック関数
    void OnCollision(const Collider* _other);

    void DebugWindow();

    Collider* GetCollider() { return &collider_; }

private:
    Rect2 boxCore_;
    Collider collider_;
    CollisionManager* pCollisionManager; // シングルトンインスタンス
};