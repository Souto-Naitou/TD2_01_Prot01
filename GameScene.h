#pragma once

#include "BaseObject.h"

#include <Vector2.h>
#include <vector>

class GameScene
{
public:
    ~GameScene();

    void Initialize();
    void Update();
    void Draw();

private:
    BaseObject* pPlayer_    = nullptr;
    BaseObject* pEnemy_     = nullptr;
    BaseObject* pCore_      = nullptr;
};