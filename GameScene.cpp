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

    originPosition_.x = 640.0f;
    originPosition_.y = 360.0f;
    radius_ = 100.0f;

    pPlayer_ = new Player();
    pPlayer_->Initialize();
    pPlayer_->SetCircle(&circle_);
}

void GameScene::Update()
{

#ifdef _DEBUG
    ImGui::Begin("debug");

    ImGui::DragFloat2("position", &originPosition_.x, 1.0f);
    ImGui::DragFloat("radius", &radius_, 1.0f);
    ImGui::InputInt("Resolution", reinterpret_cast<int*>(&resolution_));

    if (!resolution_) { resolution_ = 1; }

    ImGui::End();
#endif // _DEBUG


    if (circle_.size() != resolution_)
    {
        circle_.resize(resolution_);
    }

    float theta = 0;
    for (int i = 0; i < resolution_; i++)
    {
        Vector2 result = {};
        theta += 2.0f / resolution_ * 3.141592f;
        result.x = originPosition_.x + std::cosf(theta) * radius_ - std::sinf(theta) * radius_;
        result.y = std::sinf(theta) * radius_ + originPosition_.y + std::cosf(theta) * radius_;
        circle_[i] = result;
    }

    pPlayer_->Update();
}

void GameScene::Draw()
{
    for (int i = 0; i < resolution_ - 1; i++)
    {
        Novice::DrawLine(
            static_cast<int>(circle_[i].x), static_cast<int>(circle_[i].y),
            static_cast<int>(circle_[i+1].x), static_cast<int>(circle_[i+1].y),
            WHITE
        );
    }
    Novice::DrawLine(
        static_cast<int>(circle_[resolution_-1].x), static_cast<int>(circle_[resolution_-1].y),
        static_cast<int>(circle_[0].x), static_cast<int>(circle_[0].y),
        WHITE
    );

    pPlayer_->Draw();
}
