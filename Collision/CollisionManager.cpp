#include "CollisionManager.h"
#include "Shape.h"
#include <Vector2.h>
#include "ImGuiDebugManager/DebugManager.h"

void CollisionManager::Initialize()
{
    DebugManager::GetInstance()->SetComponent("CollisionManager", std::bind(&CollisionManager::DebugWindow, this));
}

void CollisionManager::CheckAllCollision()
{
    collisionNames_.clear();
    auto itrA = colliders_.begin();
    for (; itrA != colliders_.end(); ++itrA)
    {
        auto itrB = itrA + 1;
        for (; itrB != colliders_.end(); ++itrB)
        {
            CheckCollisionPair(*itrA, *itrB);
        }
    }
}

void CollisionManager::RegisterCollider(Collider* _collider)
{
    colliders_.push_back(_collider);
}

void CollisionManager::ClearColliderList()
{
    colliders_.clear();
}

int32_t CollisionManager::GetNewAttribute(std::string _id)
{
    for (auto& attributePair : attributeList_)
    {
        if (attributePair.first.compare(_id))
        {
            return attributePair.second;
        }
    }
    attributeList_.push_back({ _id, static_cast<uint32_t>(attributeList_.size()) + 1 });

    return attributeList_.back().second;
}

void CollisionManager::DebugWindow()
{
    if (ImGui::BeginTable("Collided list", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("ColliderA");
        ImGui::TableSetupColumn("ColliderB");
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        for (auto& cpair : collisionNames_)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::Text(cpair.first.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(cpair.second.c_str());
        }

        ImGui::EndTable();
    }
}

void CollisionManager::CheckCollisionPair(Collider* _colA, Collider* _colB)
{
    // 衝突しているかどうか
    bool isCollide = true;
    // 衝突フィルタリング
    bool fillterFlag =
        !(_colA->GetCollisionAttribute() & _colB->GetCollisionMask()) ||
        !(_colB->GetCollisionAttribute() & _colA->GetCollisionMask());
    if (fillterFlag) return;

    // 形状条件
    if (_colA->GetShape() == Shape::Polygon && _colB->GetShape() == Shape::Polygon)
    {
        std::vector<Vector2>* pVerticesA = _colA->GetVertices();
        std::vector<Vector2>* pVerticesB = _colB->GetVertices();

        // Aのすべての分離軸でチェック
        for (size_t i = 0; i < pVerticesA->size(); i++)
        {
            Vector2 edge = (*pVerticesA)[i] - (*pVerticesA)[(i + 1) % pVerticesA->size()];
            Vector2 axis = edge.Perpendicular().Normalize();
            // col1とcol2の投影をして、最小値・最大値を求める
            float minA, maxA, minB, maxB;
            ProjectShapeOnAxis(pVerticesA, axis, minA, maxA);
            ProjectShapeOnAxis(pVerticesB, axis, minB, maxB);

            if (maxA < minB || maxB < minA)
            {
                isCollide = false;
                break;
            }
        }
    }

    if (isCollide)
    {
        _colA->OnCollision(_colB);
        _colB->OnCollision(_colA);
        collisionNames_.push_back({ _colA->GetColliderID(), _colB->GetColliderID() });
    }
    return;
}

void CollisionManager::ProjectShapeOnAxis(std::vector<Vector2>* _v, const Vector2& _axis, float& _min, float& _max)
{
    _min = (*_v)[0].Projection(_axis);
    _max = _min;

    for (int i = 1; i < _v->size(); i++)
    {
        float projection = (*_v)[i].Projection(_axis);
        if (projection < _min) _min = projection;
        if (projection > _max) _max = projection;
    }
    return;
}
