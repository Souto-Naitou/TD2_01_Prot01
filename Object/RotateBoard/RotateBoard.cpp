#include "RotateBoard.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "ImGuiTemplates.h"
#include <Novice.h>

RotateBoard::RotateBoard()
{
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
    padding_ = 3;
}

void RotateBoard::RunSetMask()
{
}

void RotateBoard::Update()
{
    UpdateCourse();

    for (auto& pointPair : points_)
    {
        // pointPair
        // first    : 現在動いている辺 (頂点番号0 -> 1 の場合は0)
        // second   : 座標

        uint32_t& numEdge = pointPair.first;
        size_t courseSize = course_.size();
        if (pEasingEdgeMove->GetIsEnd())
        {
            pEasingEdgeMove->Reset();
            pEasingEdgeMove->Start();
            numEdge = (numEdge + 1) % courseSize;
        }
        pointPair.second.Lerp(course_[numEdge], course_[(numEdge + 1u) % courseSize], pEasingEdgeMove->Update());
    }

}

void RotateBoard::Draw()
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

    Novice::DrawEllipse(
        static_cast<int>(points_[0].second.x),
        static_cast<int>(points_[0].second.y),
        10, 10,
        0.0f,
        GREEN,
        kFillModeSolid
    );

}

void RotateBoard::SetVertices(const std::vector<Vector2>* _vertices)
{
    parentVertices_ = _vertices;
}

void RotateBoard::DebugWindow()
{
    ImGui::InputInt("Padding", &padding_);

    auto pFunc = [&]()
    {
        for (int i = 0; i < course_.size(); i++)
        {
            std::string name = "course_[" + std::to_string(i) + "]";
            ImGuiTemplate::VariableTableRow(name, course_[i]);
        }
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
