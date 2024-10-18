#include "Enemy.h"
#include "imgui.h"
#include "ImGuiTemplates.h"
#include "imgui.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "InputCenter.h"
#include "Player.h"
#include <Novice.h>
#include "Collision/CollisionManager.h"
#include "DefaultSettings.h"


Enemy::~Enemy()
{
    pCollisionManager_->DeleteCollider(&collider_);
    DebugManager::GetInstance()->DeleteComponent("Enemy", idx_.c_str());
}

void Enemy::Initialize(size_t idx)
{
    /// コライダーの登録
    pCollisionManager_ = CollisionManager::GetInstance();
    pCollisionManager_->RegisterCollider(&collider_);


    /// デバッグ用ウィンドウの登録
    idx_ = std::to_string(idx);        // 引数から受け取った数を文字に
    objectID_ = "Enemy" + idx_;             //"Enemy1", "Enemy2" など
    DebugManager::GetInstance()->SetComponent("Enemy", idx_, std::bind(&Enemy::DebugWindow, this));


    /// キー情報ポインタの取得
    keys_       = InputCenter::GetInstance()->GetKeyPtr();
    preKeys_    = InputCenter::GetInstance()->GetKeyPtr();


    /// 頂点の計算に必要なデータを入力
    radius_ = 20.0f;
    ellipseAB_ = { 20.0f ,10.0f };          // ax^2 + by^2 = 1


    collider_.SetColliderID(objectID_);     // コライダーのID
    moveSpeed_ = 1.0f;                      // 移動スピード


    /// コライダーの設定
    collider_.SetOwner(this);
    collider_.SetAttribute(pCollisionManager_->GetNewAttribute("Enemy"));
    collider_.SetRadius(static_cast<int>(radius_));
    collider_.SetVertices(vertices_, 3);
    // Colliderにポインタを渡す
    collider_.SetOnCollision(std::bind(&Enemy::OnCollision, this, std::placeholders::_1));
}

void Enemy::Update()
{

    /// 早期リターン
    if (isDead_) return;
    if (outScreenPadding_)
    {
        if (position_.x < -outScreenPadding_ ||
            position_.x > DefaultSettings::kScreenWidth + outScreenPadding_ ||
            position_.y < -outScreenPadding_ ||
            position_.y > DefaultSettings::kScreenHeight + outScreenPadding_)
        {
            isOutOfScreen_ = true;
            isDead_ = true;
            return;
        }
    }
    // 衝突後の動き
    if (isBouncing_) {
        // 反発する方向に移動
        position_ += distanceToTarget.Normalize() * bounceSpeed_;
    }
    else
    {
        /// 通常の移動処理
        distanceToTarget = positionTarget_ - position_;
        if (distanceToTarget.Length() > 0)
        {
            position_ += distanceToTarget.Normalize() * moveSpeed_;
        }

        if (distanceToTarget.x != 0 || distanceToTarget.y != 0)
        {
            rotation_ = std::atan2(distanceToTarget.y, distanceToTarget.x);
        }
    }


    /// ** ここより下ではPositionを更新しない **


    /// 頂点の計算
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

    if (collider_.GetIsEnableLighter()) collider_.SetPosition(position_);

    return;
}

void Enemy::Draw()
{

#ifndef _DEBUG
    /// 早期リターン (Debug時は確認のため無効)
    if (isDead_) return;
#endif // !_DEBUG
    if (isOutOfScreen_) return;

    /// 3頂点の描画 (Enemy本体)
    Novice::DrawTriangle(
        static_cast<int>(vertices_[0].x), static_cast<int>(vertices_[0].y),
        static_cast<int>(vertices_[1].x), static_cast<int>(vertices_[1].y),
        static_cast<int>(vertices_[2].x), static_cast<int>(vertices_[2].y),
        RED,
        kFillModeSolid
    );

    return;
}

void Enemy::RunSetMask()
{
    collider_.SetMask(pCollisionManager_->GetNewMask(collider_.GetColliderID()));
}

void Enemy::DebugWindow()
{
    auto pFunc = [&]()
    {
        ImGuiTemplate::VariableTableRow("Position", position_);
        ImGuiTemplate::VariableTableRow("DistanceToTarget", distanceToTarget);
        ImGuiTemplate::VariableTableRow("isDead_", isDead_);
        ImGuiTemplate::VariableTableRow("rotation_", rotation_);
        ImGuiTemplate::VariableTableRow("Attribute", collider_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("Mask", collider_.GetCollisionMask());
    };

    ImGui::DragFloat("移動速度", &moveSpeed_, 0.01f, 0.0f);

    ImGuiTemplate::VariableTable("Enemy", pFunc);

    return;
}

void Enemy::OnCollision(const Collider* _other)
{
    /// Playerとの当たり判定
    if (_other->GetColliderID() == "Player")
    {
        if (!hasCollided_)                  // 初回の衝突時
        {
            /// 向き反転
            distanceToTarget    = -distanceToTarget;
            rotation_           += 3.141592f;

            isBouncing_         = true;     //ぶっ飛びフラグオン
            hasCollided_        = true;     //衝突フラグオン
        }
        /// 二回目以降の衝突は無視

    }
    /// Coreとの当たり判定
    else if (_other->GetColliderID()== "Core")
    {
        // エネミーのデスフラグをオンに
        isDead_ = true;
    }
    //敵同士の当たり判定
    else if (_other->GetColliderID() == "Enemy") {

    }}

void Enemy::SetEnableLighter(bool _flag)
{
    collider_.SetEnableLighter(_flag);
    if (_flag)
    {
        outScreenPadding_ = 50;
    }
}
