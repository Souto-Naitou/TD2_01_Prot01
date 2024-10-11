#pragma once

#include <Vector2.h>
#include <vector>
#include "Player.h"
#include "Object/Enemy/Enemy.h"

class GameScene
{
public:
    void Initialize();
    void Update();
    void Draw();

private:
    Player* pPlayer_    = nullptr;
    Enemy*  pEnemy_     = nullptr;
};