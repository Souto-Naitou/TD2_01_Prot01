#include "Core.h"

#include "DefaultSettings.h"
#include <Novice.h>
#include "Collision/CollisionManager.h"
#include "ImGuiTemplates.h"
#include "ImGuiDebugManager/DebugManager.h"

Core::Core()
{
    pCollisionManager = CollisionManager::GetInstance();
    DebugManager::GetInstance()->SetComponent("Core", std::bind(&Core::DebugWindow, this));
}

Core::~Core()
{
    DebugManager::GetInstance()->DeleteComponent("Core");
}

void Core::Initialize()
{
    position_ = { DefaultSettings::kScreenWidth / 2u, DefaultSettings::kScreenHeight / 2u };
    boxCore_.MakeSquare(30);

    hp_ = 3;

    std::vector<Vector2> temp = boxCore_.GetVertices();
    for (auto& v : temp)
    {
        v += position_;
    }
    collider_.SetVertices(std::move(temp));

    /// コライダー関連
    // 所有者を登録
    collider_.SetOwner(this);

    // コライダー識別子を登録
    collider_.SetColliderID("Core");

    // アトリビュートの生成・登録
    collider_.SetAttribute(pCollisionManager->GetNewAttribute("Core"));

    // OnCollision関数を登録
    collider_.SetOnCollision(std::bind(&Core::OnCollision, this, std::placeholders::_1));

    // 衝突用座標の設定 (ラグ軽減用)
    collider_.SetPosition(position_);

    // 衝突半径の設定 (ラグ軽減用)
    collider_.SetRadius(static_cast<int>(30 * 1.414f));

    // ラグ軽減の有無
    collider_.SetEnableLighter(false);

    // Colliderの登録
    pCollisionManager->RegisterCollider(&collider_);
}

void Core::RunSetMask()
{
    collider_.SetMask(pCollisionManager->GetNewMask(collider_.GetColliderID(), "Player", "NestWall"));
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

void Core::OnCollision(const Collider* _other)
{
    if (_other->GetColliderID() == "Enemy")
    {
        hp_--;
    }

    if (hp_ < 0) hp_ = 0;
}

void Core::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Attribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("Mask", collider_.GetCollisionMask());
        ImGuiTemplate::VariableTableRow("hp_", hp_);
    };

    ImGuiTemplate::VariableTable("Player", pFunc);
}
