#include "Player.h"

#include "ImGuiDebugManager/DebugManager.h"
#include "Enemy.h"
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

    position_.x = 640.0f;
    position_.y = 360.0f;
    radius_current_ = radius_default_;
    collider_.SetColliderID("Player");
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

    if (vertices_.size() != resolution_)
    {
        vertices_.resize(resolution_);
    }

    float theta = 0;
    for (int i = 0; i < resolution_; i++)
    {
        Vector2 result = {};
        theta += 2.0f / resolution_ * 3.141592f;
        result.x = position_.x + std::cosf(theta) * radius_current_ - std::sinf(theta) * radius_current_;
        result.y = std::sinf(theta) * radius_current_ + position_.y + std::cosf(theta) * radius_current_;
        vertices_[i] = result;
    }

    collider_.SetVertices(&vertices_);
}

void Player::Draw()
{
    for (int i = 0; i < resolution_ - 1; i++)
    {
        Novice::DrawLine(
            static_cast<int>(vertices_[i].x), static_cast<int>(vertices_[i].y),
            static_cast<int>(vertices_[i + 1].x), static_cast<int>(vertices_[i + 1].y),
            WHITE
        );
    }
    Novice::DrawLine(
        static_cast<int>(vertices_[resolution_ - 1].x), static_cast<int>(vertices_[resolution_ - 1].y),
        static_cast<int>(vertices_[0].x), static_cast<int>(vertices_[0].y),
        WHITE
    );
}

void Player::DebugWindow()
{
#ifdef _DEBUG

    ImGui::DragFloat2("座標", &position_.x, 1.0f);
    ImGui::InputInt("頂点数", reinterpret_cast<int*>(&resolution_));
    if (!resolution_) { resolution_ = 1; }

    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("circle_", vertices_);
        ImGuiTemplate::VariableTableRow("radius_current_", radius_current_);
        ImGuiTemplate::VariableTableRow("radius_timeRelease_", radius_timeRelease_);
        ImGuiTemplate::VariableTableRow("radius_default_", radius_default_);
        ImGuiTemplate::VariableTableRow("radius_min_", radius_min_);
    };

    ImGuiTemplate::VariableTable("Player", pFunc);

#endif // _DEBUG
}

void Player::OnCollision(const Collider* _other) {
    //_otherがEnemyかどうかを確認
    if (_other->GetColliderID() == "Enemy") {
        //Enemyとの衝突処理

    }
}