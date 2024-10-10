#include "Player.h"

#include "ImGuiDebugManager/DebugManager.h"
#include <Novice.h>
#include <type_traits>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


Player::Player()
{
    DebugManager::GetInstance()->SetComponent("Player", std::bind(&Player::DebugWindow, this));
}

Player::~Player()
{
    DebugManager::GetInstance()->DeleteComponent("Player");
}

void Player::Initialize()
{
    startTime_ = std::chrono::system_clock::now();
    pEasingBoxResize_ = std::make_unique<Easing>("DecreaseSize", Easing::EaseType::EaseOutCubic, 0.5);
    pEasingBoxTemp_ = std::make_unique<Easing>("IncreaseSize", Easing::EaseType::EaseOutCubic, 0.5);

    originPosition_.x = 640.0f;
    originPosition_.y = 360.0f;
    radius_current_ = radius_default_;
}

void Player::Update()
{
    memcpy(preKeys, keys, 256);
    Novice::GetHitKeyStateAll(keys);

    if (keys[DIK_SPACE])
    {
        if (radius_current_ > radius_min_)
        {
            pEasingBoxTemp_->Reset();
            pEasingBoxResize_->Start();
            radius_current_ = (1.0f - pEasingBoxResize_->Update()) * radius_default_ + pEasingBoxResize_->Update() * radius_min_;
        }
    }
    else if (!keys[DIK_SPACE] && preKeys[DIK_SPACE])
    {
        radius_timeRelease_ = radius_current_;
    }
    else
    {
        pEasingBoxResize_->Reset();
        pEasingBoxTemp_->Start();
        radius_current_ = (1.0f - pEasingBoxTemp_->Update()) * radius_timeRelease_ + pEasingBoxTemp_->Update() * radius_default_;
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
        result.x = originPosition_.x + std::cosf(theta) * radius_current_ - std::sinf(theta) * radius_current_;
        result.y = std::sinf(theta) * radius_current_ + originPosition_.y + std::cosf(theta) * radius_current_;
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

void Player::DebugWindow()
{
#ifdef _DEBUG

    ImGui::DragFloat2("座標", &originPosition_.x, 1.0f);    
    ImGui::InputInt("頂点数", reinterpret_cast<int*>(&resolution_));
    if (!resolution_) { resolution_ = 1; }

    if (ImGui::BeginTable("Variables", 2, ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders))
    {
        ImGui::TableSetupColumn("変数名");
        ImGui::TableSetupColumn("値");
        ImGui::TableHeadersRow();

        ImGuiTemplate::VariableTableRow("circle_", circle_);
        ImGuiTemplate::VariableTableRow("radius_current_", radius_current_);
        ImGuiTemplate::VariableTableRow("radius_timeRelease_", radius_timeRelease_);
        ImGuiTemplate::VariableTableRow("radius_default_", radius_default_);
        ImGuiTemplate::VariableTableRow("radius_min_", radius_min_);

        ImGui::EndTable();
    }

#endif // _DEBUG
}