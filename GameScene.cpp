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

    delete pPlayer_; pPlayer_ = nullptr;
    delete pEnemy_; pEnemy_ = nullptr;
    delete pCore_; pCore_ = nullptr;
    delete pNestWallLeft_; pNestWallLeft_ = nullptr;
}

void GameScene::Initialize()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG

    pCollisionManager_ = CollisionManager::GetInstance();
    pCollisionManager_->Initialize();

    pPlayer_ = new Player();
    pPlayer_->Initialize();

    pEnemy_ = new Enemy();
    pEnemy_->Initialize();

    pCore_ = new Core();
    pCore_->Initialize();

    MakeWall(&pNestWallLeft_, "NestWallLeft", 40, 720, { 0,0 });
    MakeWall(&pNestWallTop_, "NestWallTop", 1280, 40, { 0,0 });
    MakeWall(&pNestWallRight_, "NestWallRight", 40, 720, { 1240,0 });
    MakeWall(&pNestWallBottom_, "NestWallBottom", 1280, 40, { 0,680 });

    static_cast<Enemy*>(pEnemy_)->SetTargetPosition(pPlayer_->GetWorldPosition());

    /// マスクの生成にアトリビュートを使用するためInitialize後に行う
    pPlayer_->RunSetMask();

    pCore_->RunSetMask();
}

void GameScene::Update()
{
    pPlayer_->Update();
    pEnemy_->Update();
    pCore_->Update();
    pNestWallLeft_->Update();
    pNestWallTop_->Update();
    pNestWallRight_->Update();
    pNestWallBottom_->Update();

    pCollisionManager_->CheckAllCollision();
}

void GameScene::Draw()
{
    pNestWallLeft_->Draw();
    pPlayer_->Draw();
    pEnemy_->Draw();
    pCore_->Draw();
    pNestWallLeft_->Draw();
    pNestWallTop_->Draw();
    pNestWallRight_->Draw();
    pNestWallBottom_->Draw();
}

void GameScene::MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin)
{
    *_nestWall = new NestWall(_id);
    (*_nestWall)->SetRect(_width, _height, _origin);
    (*_nestWall)->Initialize();
    pCollisionManager_->RegisterCollider((*_nestWall)->GetCollider());
    (*_nestWall)->GetCollider()->SetAttribute(pCollisionManager_->GetNewAttribute("NestWall"));
}
