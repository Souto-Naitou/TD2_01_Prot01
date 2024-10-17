#include "NestWall.h"
#include <Novice.h>
#include "ImGuiDebugManager/DebugManager.h"
#include "ImGuiTemplates.h"

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
    collider_.SetColliderID("NestWall");
    collider_.SetAttribute(pCollisionManager_->GetNewAttribute("NestWall"));
    pCollisionManager_->RegisterCollider(&collider_);

    // コライダーにOnCollisionの関数ポインタを渡す
    collider_.SetOnCollision(std::bind(&NestWall::OnCollision, this, std::placeholders::_1));
    collider_.SetOwner(this);
}

void NestWall::RunSetMask()
{
    collider_.SetMask(pCollisionManager_->GetNewMask(collider_.GetColliderID(), "Player", "Core"));
}

void NestWall::Update()
{
}

void NestWall::Draw()
{
    Rect2 newRect = rect_ + position_;
    Novice::DrawBox(newRect.x1, newRect.y1, newRect.x2, newRect.y2, 0.0f, BLUE, kFillModeWireFrame);
}

void NestWall::OnCollision(const Collider* _collider)
{
    if (_collider->GetColliderID() == "Enemy")
    {
        hp_--;
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
