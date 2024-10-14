#include "Core.h"
#include <Novice.h>
#include "Collision/CollisionManager.h"
#include "ImGuiTemplates.h"
#include "ImGuiDebugManager/DebugManager.h"

Core::Core()
{
    DebugManager::GetInstance()->SetComponent("Core", std::bind(&Core::DebugWindow, this));
}

Core::~Core()
{
    DebugManager::GetInstance()->DeleteComponent("Core");
}

void Core::Initialize()
{
    CollisionManager* pCollisionManager = CollisionManager::GetInstance();
    position_ = { 640, 360 };
    boxCore_.MakeSquare(30);
    collider_.SetColliderID("Core");
    pCollisionManager->RegisterCollider(&collider_);
    collider_.SetAttribute(pCollisionManager->GetNewAttribute("Core"));
  
    std::vector<Vector2> temp = boxCore_.GetVertices();
    for (auto& v : temp)
    {
        v += position_;
    }
    collider_.SetVertices(std::move(temp));
}

void Core::Update()
{
}

void Core::Draw()
{
    Rect2 coreDrawn = boxCore_ + position_;
    Novice::DrawBox(
        coreDrawn.x1, coreDrawn.y1,
        coreDrawn.GetSize(), coreDrawn.GetSize(),
        0.0f,
        GREEN,
        kFillModeSolid
    );
}

void Core::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Attribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("Mask", collider_.GetCollisionMask());
    };

    ImGuiTemplate::VariableTable("Player", pFunc);
}
