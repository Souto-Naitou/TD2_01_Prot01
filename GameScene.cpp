#include "GameScene.h"

#include <Novice.h>

#include <cmath>
#include <numbers>

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

void GameScene::Initialize()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG

    pPlayer_ = new Player();
    pPlayer_->Initialize();
}

void GameScene::Update()
{
    pPlayer_->Update();
}

void GameScene::Draw()
{
    pPlayer_->Draw();
}
