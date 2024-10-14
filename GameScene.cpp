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
    delete pCollisionManager_; pCollisionManager_ = nullptr;
}

void GameScene::Initialize()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG

    pCollisionManager_ = new CollisionManager();
    pCollisionManager_->Initialize();

    pPlayer_ = new Player();
    pPlayer_->Initialize();
    pCollisionManager_->RegisterCollider(pPlayer_->GetCollider());
    //Playerの属性とマスクの設定
    uint32_t playerAttribute = pCollisionManager_->GetNewAttribute("Player");
    pPlayer_->GetCollider()->SetAttribute(playerAttribute);
    pPlayer_->GetCollider()->SetMask(~playerAttribute); 
   // pPlayer_->GetCollider()->SetAttribute(pCollisionManager_->GetNewAttribute("Player"));


    pEnemy_ = new Enemy();
    pEnemy_->Initialize();
    pCollisionManager_->RegisterCollider(pEnemy_->GetCollider());
    pEnemy_->GetCollider()->SetAttribute(pCollisionManager_->GetNewAttribute("Enemy"));

    pCore_ = new Core();
    pCore_->Initialize();
    pCollisionManager_->RegisterCollider(pCore_->GetCollider());
    // Core の属性とマスクの設定
    uint32_t coreAttribute = pCollisionManager_->GetNewAttribute("Core");
    pCore_->GetCollider()->SetAttribute(coreAttribute);
    pCore_->GetCollider()->SetMask(~coreAttribute);
   // pCore_->GetCollider()->SetAttribute(pCollisionManager_->GetNewAttribute("Core"));

    static_cast<Enemy*>(pEnemy_)->SetTargetPosition(pPlayer_->GetWorldPosition());
}

void GameScene::Update()
{
    pPlayer_->Update();
    pEnemy_->Update();
    pCore_->Update();

    pCollisionManager_->CheckAllCollision();
}

void GameScene::Draw()
{
    pPlayer_->Draw();
    pEnemy_->Draw();
    pCore_->Draw();
}
