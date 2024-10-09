#pragma once

#include <Vector2.h>
#include <vector>
#include "Player.h"

class GameScene
{
public:
    void Initialize();
    void Update();
    void Draw();

private:
    Player* pPlayer_ = nullptr;
};