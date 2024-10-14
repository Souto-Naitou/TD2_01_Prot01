#include "Enemy.h"
#include "imgui.h"
#include "ImGuiTemplates.h"
#include "imgui.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "InputCenter.h"
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

    pCollisionManager->RegisterCollider(&collider_);
    collider_.SetAttribute(pCollisionManager->GetNewAttribute("Enemy"));
}

void Enemy::Update()
{
    distanceToTarget = positionTarget_ - position_;
    if (distanceToTarget.x != 0)
        rotation_ = std::atan2(distanceToTarget.y , distanceToTarget.x);

    InputCenter::GetInstance()->WASDMove(position_, 3.0f);

    float theta = 0;
    //for (int i = 0; i < 3; i++)
    //{
    //    Vector2 result = {};
    //    theta += 2.0f / 3 * 3.141592f;
    //    result.x = position_.x + std::cosf(theta + rotation_) * radius_;
    //    result.y = position_.y + std::sinf(theta + rotation_) * radius_;
    //    vertices[i] = result;
    //}

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

    ImGuiTemplate::VariableTable("Enemy", pFunc);

    return;
}
