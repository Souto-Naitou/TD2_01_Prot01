#pragma once

#include "BaseObject.h"

#include <Vector2.h>
#include <vector>

class GameScene
{
public:
    void Initialize();
    void Update();
    void Draw();

private:
    BaseObject* pPlayer_    = nullptr;
    BaseObject* pEnemy_     = nullptr;
};