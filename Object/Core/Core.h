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
    void Update();
    void Draw();

    void DebugWindow();

    Collider* GetCollider() { return &collider_; }

private:
    Rect2 boxCore_;
    Collider collider_;
};