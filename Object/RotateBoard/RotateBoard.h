#pragma once

#include "easing/Easing.h"
#include <Vector2.h>
#include <Rect2.h>
#include <vector>
#include <array>
#include <memory>

class RotateBoard
{
public:

    RotateBoard();
    ~RotateBoard();

    void Initialize();
    void RunSetMask();
    void Update();
    void Draw();

    void SetVertices(const std::vector<Vector2>* _vertices);

    void DebugWindow();

private:
    Rect2 rect_;
    const std::vector<Vector2>* parentVertices_;
    std::vector<Vector2> course_;
    int32_t padding_ = 0;
    std::array<std::pair<uint32_t, Vector2>, 2> points_;
    std::unique_ptr<Easing> pEasingEdgeMove = nullptr;
    bool isDrawCourse_ = false;
    bool isDrawPoints_ = false;
    float offset_ = 0.0f;
    float t1, t2;

private:
    void UpdateCourse();
    void DrawPoints();
    void DrawCourse();
    void DrawBoardLine();
};