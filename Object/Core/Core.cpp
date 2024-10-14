#include "Core.h"
#include <Novice.h>

void Core::Initialize()
{
    position_ = { 640, 360 };
    boxCore_.MakeSquare(30);
    collider_.SetColliderID("Core");
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