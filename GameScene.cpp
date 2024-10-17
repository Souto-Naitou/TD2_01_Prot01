#include "GameScene.h"

#include <Novice.h>
#include "Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"

#include <cmath>
#include <numbers>

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

GameScene::~GameScene()
{
    pCollisionManager_->ClearColliderList();

    delete pCore_; pCore_ = nullptr;
    delete pNestWallLeft_; pNestWallLeft_ = nullptr;

    for (Enemy* ptr : enemyList_)
    {
        delete ptr;
        ptr = nullptr;
    }
}

void GameScene::Initialize()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG

    pCollisionManager_ = CollisionManager::GetInstance();
    pCollisionManager_->Initialize();

    pEnemyPopSystem_ = new EnemyPopSystem();

    pPlayer_ = new Player();
    pPlayer_->Initialize();

    pCore_ = new Core();
    pCore_->Initialize();

    MakeWall(&pNestWallLeft_, "Left", 40, 720, { 0,0 });
    MakeWall(&pNestWallTop_, "Top", 1280, 40, { 0,0 });
    MakeWall(&pNestWallRight_, "Right", 40, 720, { 1240,0 });
    MakeWall(&pNestWallBottom_, "Bottom", 1280, 40, { 0,680 });

    /// マスクの生成にアトリビュートを使用するためInitialize後に行う
    pPlayer_->RunSetMask();

    pCore_->RunSetMask();
    pNestWallLeft_->RunSetMask();
    pNestWallTop_->RunSetMask();
    pNestWallRight_->RunSetMask();
    pNestWallBottom_->RunSetMask();

    timer_.Start();
}

void GameScene::Update()
{
    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }

    if (timer_.GetNow() > 60.0)
    {
        timer_.Reset();
        for (Enemy* ptr : enemyList_)
        {
            delete ptr;
        }
        enemyList_.clear();
    }

    pPlayer_->Update();
    pCore_->Update();
    pNestWallLeft_->Update();
    pNestWallTop_->Update();
    pNestWallRight_->Update();
    pNestWallBottom_->Update();

    pCollisionManager_->CheckAllCollision();
    Enemy* ptrEnemy = pEnemyPopSystem_->Update(0.5, { 0,0 }, { 1280,720 });
    if (ptrEnemy)
    {
        ptrEnemy->Initialize(enemyList_.size());
        ptrEnemy->SetTargetPosition(pPlayer_->GetWorldPosition());
        enemyList_.push_back(ptrEnemy);
    }
    for (Enemy* ptr : enemyList_) ptr->Update();
}

void GameScene::Draw()
{
    Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x242424ff, kFillModeSolid);

    pNestWallLeft_->Draw();
    pPlayer_->Draw();
    pCore_->Draw();
    pNestWallLeft_->Draw();
    pNestWallTop_->Draw();
    pNestWallRight_->Draw();
    pNestWallBottom_->Draw();
    for (Enemy* ptr : enemyList_) ptr->Draw();

}

void GameScene::MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin)
{
    *_nestWall = new NestWall(_id);
    (*_nestWall)->SetRect(_width, _height, _origin);
    (*_nestWall)->Initialize();
}
