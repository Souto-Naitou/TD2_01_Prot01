#include "Enemy.h"
#include "imgui.h"
#include "ImGuiTemplates.h"
#include "imgui.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "InputCenter.h"
#include "Player.h"
#include <Novice.h>
#include "Collision/CollisionManager.h"


void Enemy::Initialize()
{
    CollisionManager* pCollisionManager = CollisionManager::GetInstance();
    DebugManager::GetInstance()->SetComponent("Enemy", std::bind(&Enemy::DebugWindow, this));
    keys_ = InputCenter::GetInstance()->GetKeyPtr();
    preKeys_ = InputCenter::GetInstance()->GetKeyPtr();

    position_ = { 640, 360 };
    radius_ = 20.0f;
    ellipseAB_ = { 20.0f ,10.0f };
    collider_.SetColliderID("Enemy");
    moveSpeed_ = 0.05f;

    pCollisionManager->RegisterCollider(&collider_);
    collider_.SetOwner(this);
    collider_.SetAttribute(pCollisionManager->GetNewAttribute("Enemy"));
    //Colliderにポインタを渡す
    collider_.SetOnCollision(std::bind(&Enemy::OnCollision, this, std::placeholders::_1));
}

void Enemy::Update()
{
    /// 距離の更新と方向の調整
    distanceToTarget = positionTarget_ - position_;
    if (distanceToTarget.x != 0 || distanceToTarget.y != 0)
        rotation_ = std::atan2(distanceToTarget.y , distanceToTarget.x);

    /// WASD移動の簡易実装
    InputCenter::GetInstance()->WASDMove(position_, 3.0f);

    /// 自動移動
    position_ += distanceToTarget.Normalize() * moveSpeed_;



    /// ここより下ではPositionを更新しない
    float theta = 0;
    Vector2 result = {};
    for (int i = 0; i < 3; i += 1)
    {
        theta += 2.0f / 3 * 3.141592f;
        result.x = ellipseAB_.x * std::cosf(theta);
        result.y = ellipseAB_.y * std::sinf(theta);
        vertices_[i] = result.Rotated(rotation_) + position_;
    }

    collider_.SetVertices(vertices_, 3);

    return;
}

void Enemy::Draw()
{
    Novice::DrawTriangle(
        static_cast<int>(vertices_[0].x), static_cast<int>(vertices_[0].y),
        static_cast<int>(vertices_[1].x), static_cast<int>(vertices_[1].y),
        static_cast<int>(vertices_[2].x), static_cast<int>(vertices_[2].y),
        RED,
        kFillModeSolid
    );

    return;
}

void Enemy::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Position", position_);
        ImGuiTemplate::VariableTableRow("DistanceToTarget", distanceToTarget);
        ImGuiTemplate::VariableTableRow("rotation_", rotation_);
        ImGuiTemplate::VariableTableRow("Attribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("Mask", collider_.GetCollisionMask());
    };

    ImGui::DragFloat("移動速度", &moveSpeed_, 0.01f, 0.0f);

    ImGuiTemplate::VariableTable("Enemy", pFunc);

    return;
}

void Enemy::OnCollision(const Collider* _other) {
    //_otherがPlayerかどうか確認
    if (_other->GetColliderID() == "Player")
    {
        // Playerとの衝突処理

    }
}
