#include "RotateBoard.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "ImGuiTemplates.h"
#include "Object/Enemy/Enemy.h"
#include <Novice.h>

RotateBoard::RotateBoard()
{
    /// インスタンス生成など
    pCollisionManager_ = CollisionManager::GetInstance();
    DebugManager::GetInstance()->SetComponent("RotateBoard", std::bind(&RotateBoard::DebugWindow, this));
    pEasingEdgeMove = std::make_unique<Easing>("RotateBoard_EdgeMove");
}

RotateBoard::~RotateBoard()
{
    // RotateBoardのDebugWindowを消去
    DebugManager::GetInstance()->DeleteComponent("RotateBoard");
}

void RotateBoard::Initialize()
{
    /// 各種データの初期化
    padding_ = 24;
    offset_ = 0.5f;


    // 回転の開始
    pEasingEdgeMove->Start();


    /// コライダー関連
    pCollisionManager_->RegisterCollider(&collider1_);
    collider1_.SetColliderID("RotateBoard");
    collider1_.SetAttribute(pCollisionManager_->GetNewAttribute("RotateBoard"));
    collider1_.SetOwner(this);
    collider1_.SetOnCollision(std::bind(&RotateBoard::OnCollision, this, std::placeholders::_1));
    collider1_.SetEnableLighter(false);

    pCollisionManager_->RegisterCollider(&collider2_);
    collider2_.SetColliderID("RotateBoard");
    collider2_.SetAttribute(pCollisionManager_->GetNewAttribute("RotateBoard"));
    collider2_.SetOwner(this);
    collider2_.SetOnCollision(std::bind(&RotateBoard::OnCollision, this, std::placeholders::_1));
    collider2_.SetEnableLighter(false);
}

void RotateBoard::RunSetMask()
{
    // マスクの設定
    collider1_.SetMask(pCollisionManager_->GetNewMask(collider1_.GetColliderID(), "Player", "Core", "NestWall"));
    collider2_.SetMask(pCollisionManager_->GetNewMask(collider2_.GetColliderID(), "Player", "Core", "NestWall"));
}

void RotateBoard::Update()
{
    // コースを更新
    UpdateCourse();

    /// 頂点の更新
    /// first    : 現在動いている辺 (頂点番号0 -> 1 の場合は0)
    /// second   : 座標

    uint32_t& numEdge1 = points_[0].first;
    uint32_t& numEdge2 = points_[1].first;
    size_t courseSize = course_.size();
    if (pEasingEdgeMove->GetIsEnd())
    {
        // 無限に回転させるために
        pEasingEdgeMove->Reset();
        pEasingEdgeMove->Start();
        numEdge1 = (numEdge1 + 1) % courseSize;
    }

    /// t1は通常回転、t2はt1をずらしたもの
    t1 = pEasingEdgeMove->Update();
    t2 = t1 + offset_;

    /// t2が1.0fを超えたら1.0fを引く
    if (t2 > 1.0f)
    {
        t2 -= 1.0f;
        // 辺番号の更新
        numEdge2 = (numEdge1 + 1) % courseSize;
    }
    else
    {
        // 辺番号の更新
        numEdge2 = numEdge1;
    }

    /// 頂点の更新
    points_[0].second.Lerp(course_[numEdge1], course_[(numEdge1 + 1u) % courseSize], t1);
    points_[1].second.Lerp(course_[numEdge2], course_[(numEdge2 + 1u) % courseSize], t2);


    /// コライダーの更新
    if (points_[0].first != points_[1].first)
    {
        verticesCollider1_[0] = points_[0].second;
        verticesCollider1_[1] = course_[points_[1].first];
        verticesCollider2_[0] = points_[1].second;
        verticesCollider2_[1] = course_[points_[1].first];
        collider1_.SetVertices(verticesCollider1_, 2);
        collider2_.SetVertices(verticesCollider2_, 2);
        isCorner_ = true;
    }
    else
    {
        verticesCollider1_[0] = points_[0].second;
        verticesCollider1_[1] = points_[1].second;
        verticesCollider2_[0] = {};
        verticesCollider2_[1] = {};
        collider1_.SetVertices(verticesCollider1_, 2);
        collider2_.SetVertices(verticesCollider2_, 0);
        isCorner_ = false;
    }
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

void RotateBoard::OnCollision(const Collider* _other)
{
    _other;
}

void RotateBoard::DebugWindow()
{
#ifdef _DEBUG

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
        ImGuiTemplate::VariableTableRow("attr", collider1_.GetCollisionAttribute());
        ImGuiTemplate::VariableTableRow("mask", collider1_.GetCollisionMask());
    };

    ImGuiTemplate::VariableTable("RotateBoard", pFunc);

#endif // _DEBUG
}

void RotateBoard::UpdateCourse()
{
    // 早期リターン
    if (!parentVertices_) return;

    // 頂点数
    size_t numVertices = parentVertices_->size();

    // 親の頂点をコースに代入
    course_ = *parentVertices_;

    /// ベクトルたちをちょっと拡張！
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

        Novice::ScreenPrintf(
            static_cast<int>(points_[i].second.x),
            static_cast<int>(points_[i].second.y),
            "[%d]", i
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
    Novice::ScreenPrintf(
        static_cast<int>(course_[points_[1].first].x),
        static_cast<int>(course_[points_[1].first].y),
        "[c]"
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
