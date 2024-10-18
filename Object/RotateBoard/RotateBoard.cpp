#include "RotateBoard.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "ImGuiTemplates.h"
#include <Novice.h>

RotateBoard::RotateBoard()
{
    pCollisionManager_ = CollisionManager::GetInstance();
    t1 = t2 = 0;
    parentVertices_ = nullptr;
    DebugManager::GetInstance()->SetComponent("RotateBoard", std::bind(&RotateBoard::DebugWindow, this));
    pEasingEdgeMove = std::make_unique<Easing>("RotateBoard_EdgeMove");
}

RotateBoard::~RotateBoard()
{
    DebugManager::GetInstance()->DeleteComponent("RotateBoard");
}

void RotateBoard::Initialize()
{
    padding_ = 24;
    offset_ = 0.5f;

    pEasingEdgeMove->Start();

    collider_.SetColliderID("RotateBoard");
    collider_.SetAttribute(pCollisionManager_->GetNewAttribute("RotateBoard"));
    collider_.SetOwner(this);
}

void RotateBoard::RunSetMask()
{
    collider_.SetMask(pCollisionManager_->GetNewMask(collider_.GetColliderID(), "Player", "Core", "NestWall"));
}

void RotateBoard::Update()
{
    UpdateCourse();

    // first    : 現在動いている辺 (頂点番号0 -> 1 の場合は0)
    // second   : 座標

    uint32_t& numEdge1 = points_[0].first;
    uint32_t& numEdge2 = points_[1].first;
    size_t courseSize = course_.size();
    if (pEasingEdgeMove->GetIsEnd())
    {
        pEasingEdgeMove->Reset();
        pEasingEdgeMove->Start();
        numEdge1 = (numEdge1 + 1) % courseSize;
    }

    t1 = t2 = 0;

    t1 = pEasingEdgeMove->Update();
    t2 = t1 + offset_;

    if (t2 > 1.0f)
    {
        t2 -= 1.0f;
        numEdge2 = (numEdge1 + 1) % courseSize;
    }
    else
    {
        numEdge2 = numEdge1;
    }

    points_[0].second.Lerp(course_[numEdge1], course_[(numEdge1 + 1u) % courseSize], t1);
    points_[1].second.Lerp(course_[numEdge2], course_[(numEdge2 + 1u) % courseSize], t2);


    verticesCollider_[0] = points_[0].second;
    verticesCollider_[1] = points_[1].second;
    collider_.SetVertices(verticesCollider_, 2);
}

void RotateBoard::Draw()
{
    if (isDrawCourse_)
    {
        DrawCourse();
    }

    if (isDrawPoints_)
    {
        DrawPoints();
    }

    DrawBoardLine();
}

void RotateBoard::SetVertices(const std::vector<Vector2>* _vertices)
{
    parentVertices_ = _vertices;
}

void RotateBoard::DebugWindow()
{
    ImGui::InputInt("Padding", &padding_);
    ImGui::DragFloat("Offset", &offset_, 0.01f);

    ImGui::Checkbox("DrawCourse", &isDrawCourse_);
    ImGui::Checkbox("isDrawPoints", &isDrawPoints_);

    auto pFunc = [&]()
    {
        for (int i = 0; i < course_.size(); i++)
        {
            std::string name = "course_[" + std::to_string(i) + "]";
            ImGuiTemplate::VariableTableRow(name, course_[i]);
        }
        ImGuiTemplate::VariableTableRow("points_[0].first", points_[0].first);
        ImGuiTemplate::VariableTableRow("points_[1].first", points_[1].first);
        ImGuiTemplate::VariableTableRow("t1", t1);
        ImGuiTemplate::VariableTableRow("t2", t2);
    };

    ImGuiTemplate::VariableTable("RotateBoard", pFunc);
}

void RotateBoard::UpdateCourse()
{
    // 早期リターン
    if (!parentVertices_) return;

    // 頂点数
    size_t numVertices = parentVertices_->size();

    course_ = *parentVertices_;

    for (size_t i = 0; i < numVertices; i++)
    {
        // 対象ベクトル
        Vector2 currentVertex = (*parentVertices_)[i];

        // 辺ベクトルを算出
        Vector2 edge = (*parentVertices_)[(i + 1) % numVertices] - currentVertex;

        course_[i] += edge.Perpendicular().Normalize() * static_cast<float>(-padding_);
        course_[(i + 1) % numVertices] += edge.Perpendicular().Normalize() * static_cast<float>(-padding_);
    }
}

void RotateBoard::DrawPoints()
{
    for (int i = 0; i < points_.size(); i++)
    {
        Novice::DrawEllipse(
            static_cast<int>(points_[i].second.x),
            static_cast<int>(points_[i].second.y),
            5, 5,
            0.0f,
            GREEN,
            kFillModeSolid
        );
    }

    Novice::DrawEllipse(
        static_cast<int>(course_[points_[1].first].x),
        static_cast<int>(course_[points_[1].first].y),
        5, 5,
        0.0f,
        BLUE,
        kFillModeSolid
    );
}

void RotateBoard::DrawCourse()
{
    size_t numCouseVertex = course_.size();
    for (size_t i = 0; i < numCouseVertex; i++)
    {
        Novice::DrawLine(
            static_cast<int>(course_[i].x),
            static_cast<int>(course_[i].y),
            static_cast<int>(course_[(i + 1) % numCouseVertex].x),
            static_cast<int>(course_[(i + 1) % numCouseVertex].y),
            GREEN
        );
    }
}

void RotateBoard::DrawBoardLine()
{
    if (points_[0].first != points_[1].first)
    {
        Novice::DrawLine(
            static_cast<int>(points_[0].second.x),
            static_cast<int>(points_[0].second.y),
            static_cast<int>(course_[points_[1].first].x),
            static_cast<int>(course_[points_[1].first].y),
            BLUE
        );
        Novice::DrawLine(
            static_cast<int>(points_[1].second.x),
            static_cast<int>(points_[1].second.y),
            static_cast<int>(course_[points_[1].first].x),
            static_cast<int>(course_[points_[1].first].y),
            BLUE
        );
    }
    else
    {
        Novice::DrawLine(
            static_cast<int>(points_[0].second.x),
            static_cast<int>(points_[0].second.y),
            static_cast<int>(points_[1].second.x),
            static_cast<int>(points_[1].second.y),
            BLUE
        );
    }
}
