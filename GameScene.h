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
    float radius_ = 15.0f;
    size_t resolution_ = 32u;
    Vector2 originPosition_ = {};
    std::vector<Vector2> circle_ = {};

    Player* pPlayer_ = nullptr;
};