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
    delete pPlayer_; pPlayer_ = nullptr;
    delete pEnemy_; pEnemy_ = nullptr;
    delete pCore_; pCore_ = nullptr;
}

void GameScene::Initialize()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG

    pPlayer_ = new Player();
    pPlayer_->Initialize();

    pEnemy_ = new Enemy();
    pEnemy_->Initialize();

    pCore_ = new Core();
    pCore_->Initialize();

    static_cast<Enemy*>(pEnemy_)->SetTargetPosition(pPlayer_->GetWorldPosition());
}

void GameScene::Update()
{
    pPlayer_->Update();
    pEnemy_->Update();
    pCore_->Update();
}

void GameScene::Draw()
{
    pPlayer_->Draw();
    pEnemy_->Draw();
    pCore_->Draw();
}
