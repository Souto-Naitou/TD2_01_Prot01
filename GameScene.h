#pragma once

#include "Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"
#include "Object/Wall/NestWall.h"
#include "Object/RotateBoard/RotateBoard.h"
#include "Collision/CollisionManager.h"

#include "EnemyPopSytem.h"
#include "Timer/Timer.h"

#include <Vector2.h>
#include <vector>

class GameScene
{
public:
    GameScene();
    ~GameScene();

    void Initialize();
    void Update();
    void Draw();

    void DebugWindow();

private:
    CollisionManager*   pCollisionManager_  = nullptr;
    Player*             pPlayer_            = nullptr;
    Core*               pCore_              = nullptr;
    NestWall*           pNestWallLeft_      = nullptr;
    NestWall*           pNestWallTop_       = nullptr;
    NestWall*           pNestWallRight_     = nullptr;
    NestWall*           pNestWallBottom_    = nullptr;
    EnemyPopSystem*     pEnemyPopSystem_    = nullptr;
    std::list<Enemy*>   enemyList_          = {};
    Timer               timer_              = {};
    bool                isEnableLighter_    = false;

    void MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin);
};