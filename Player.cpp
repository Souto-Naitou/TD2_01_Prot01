#include "Player.h"

#include <Novice.h>
#include <imgui.h>

void Player::Initialize()
{
    startTime_ = std::chrono::system_clock::now();
    pEasingDecreaseSize_ = std::make_unique<Easing>("DecreaseSize", Easing::EaseType::EaseOutCubic, 0.5);
    pEasingIncreaseSize_ = std::make_unique<Easing>("IncreaseSize", Easing::EaseType::EaseOutCubic, 0.5);

    originPosition_.x = 640.0f;
    originPosition_.y = 360.0f;
    radius_ = radius_default;
}

void Player::Update()
{
#ifdef _DEBUG
    ImGui::Begin("debug");

    ImGui::DragFloat2("position", &originPosition_.x, 1.0f);
    ImGui::DragFloat("radius", &radius_, 1.0f);
    ImGui::InputInt("Resolution", reinterpret_cast<int*>(&resolution_));

    if (!resolution_) { resolution_ = 1; }

    ImGui::End();
#endif // _DEBUG

    memcpy(preKeys, keys, 256);
    Novice::GetHitKeyStateAll(keys);

    if (keys[DIK_SPACE])
    {
        if (radius_ >= radius_min)
        {
            pEasingIncreaseSize_->Reset();
            pEasingDecreaseSize_->Start();
            radius_ = (1.0f - pEasingDecreaseSize_->Update()) * radius_default + pEasingDecreaseSize_->Update() * radius_min;
        }
    }
    else
    {
        pEasingDecreaseSize_->Reset();
        pEasingIncreaseSize_->Start();
        radius_ = (1.0f - pEasingIncreaseSize_->Update()) * radius_min + pEasingIncreaseSize_->Update() * radius_default;
    }

    std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();

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
}

void Player::Draw()
{
    for (int i = 0; i < resolution_ - 1; i++)
    {
        Novice::DrawLine(
            static_cast<int>(circle_[i].x), static_cast<int>(circle_[i].y),
            static_cast<int>(circle_[i + 1].x), static_cast<int>(circle_[i + 1].y),
            WHITE
        );
    }
    Novice::DrawLine(
        static_cast<int>(circle_[resolution_ - 1].x), static_cast<int>(circle_[resolution_ - 1].y),
        static_cast<int>(circle_[0].x), static_cast<int>(circle_[0].y),
        WHITE
    );
}