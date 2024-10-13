#pragma once

#include "Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"
#include "Collision/CollisionManager.h"

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
    CollisionManager*   pCollisionManager_  = nullptr;
    Player*         pPlayer_            = nullptr;
    Enemy*         pEnemy_             = nullptr;
    Core*         pCore_              = nullptr;
};