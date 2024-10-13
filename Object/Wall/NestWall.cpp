#include "NestWall.h"
#include <Novice.h>
#include "ImGuiDebugManager/DebugManager.h"
#include "ImGuiTemplates.h"

NestWall::NestWall(std::string _ID)
{
    id_ = _ID;
    DebugManager::GetInstance()->SetComponent(_ID.c_str(), std::bind(&NestWall::DebugWindow, this));
}

NestWall::~NestWall()
{
    DebugManager::GetInstance()->DeleteComponent(id_.c_str());
}

void NestWall::Initialize()
{
    collider_.SetColliderID("NestWall");
}

void NestWall::Update()
{
}

void NestWall::Draw()
{
    Novice::DrawBox(rect_.x1, rect_.y1, rect_.x2, rect_.y2, 0.0f, BLUE, kFillModeWireFrame);
}

void NestWall::SetRect(int _width, int _height, Vector2 _origin)
{
    rect_.MakeRectangle(_width, _height, false);
    collider_.SetVertices(rect_.GetVertices());
    position_ = _origin;
    return;
}

void NestWall::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("id_", id_.c_str());
        ImGuiTemplate::VariableTableRow("rect_.LeftTop", rect_.LeftTop());
        ImGuiTemplate::VariableTableRow("rect_.RightBottom", rect_.RightBottom());
        ImGuiTemplate::VariableTableRow("collider_.GetCollisionAttribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("collider_.GetCollisionMask", collider_.GetCollisionMask());
    };

    ImGuiTemplate::VariableTable(id_.c_str(), pFunc);
}
