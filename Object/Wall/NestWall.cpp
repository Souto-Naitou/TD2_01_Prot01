#include "NestWall.h"
#include <Novice.h>
#include "ImGuiDebugManager/DebugManager.h"
#include "ImGuiTemplates.h"
#include "Object/Enemy/Enemy.h"

NestWall::NestWall(std::string _ID)
{
    objectID_ = _ID;
    DebugManager::GetInstance()->SetComponent("NestWall", _ID.c_str(), std::bind(&NestWall::DebugWindow, this));
    pCollisionManager_ = CollisionManager::GetInstance();
}

NestWall::~NestWall()
{
    DebugManager::GetInstance()->DeleteComponent(objectID_.c_str());
}

void NestWall::Initialize()
{
    // 仮HP
    hp_ = 100u;
    collider_.SetColliderID("NestWall_" + objectID_);
    collider_.SetAttribute(pCollisionManager_->GetNewAttribute("NestWall"));
    pCollisionManager_->RegisterCollider(&collider_);

    // コライダーにOnCollisionの関数ポインタを渡す
    collider_.SetOnCollisionTrigger(std::bind(&NestWall::OnCollisionTrigger, this, std::placeholders::_1));
    collider_.SetOwner(this);
}

void NestWall::RunSetMask()
{
    collider_.SetMask(pCollisionManager_->GetNewMask("NestWall", "Player", "Core"));
}

void NestWall::Update()
{
    if (hp_ == 0) isDead_ = true;
}

void NestWall::Draw()
{
    if (isDead_) return;
    Novice::DrawBox(
        static_cast<int>(position_.x), static_cast<int>(position_.y), rect_.x2, rect_.y2, 0.0f, RED, kFillModeSolid);
}

void NestWall::OnCollisionTrigger(const Collider* _collider)
{
    if (_collider->GetColliderID() == "Enemy")
    {
        const Enemy* pEnemy = static_cast<const Enemy*>(_collider->GetOwner());
        if (pEnemy->GetIsBounce() && !pEnemy->GetIsCollidedNest())
        {
            hp_--;
        }
    }

    if (hp_ < 0) hp_ = 0;
}

void NestWall::SetRect(int _width, int _height, Vector2 _leftTop)
{
    rect_.MakeRectangle(_width, _height, false);
    collider_.SetVertices((rect_ + _leftTop).GetVertices());
    position_ = _leftTop;
    return;
}

void NestWall::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("id_", objectID_);
        ImGuiTemplate::VariableTableRow("hp_", hp_);
        ImGuiTemplate::VariableTableRow("rect_.LeftTop", rect_.LeftTop());
        ImGuiTemplate::VariableTableRow("rect_.RightBottom", rect_.RightBottom());
        ImGuiTemplate::VariableTableRow("collider_.GetCollisionAttribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("collider_.GetCollisionMask", collider_.GetCollisionMask());
    };

    ImGuiTemplate::VariableTable(objectID_.c_str(), pFunc);
}
