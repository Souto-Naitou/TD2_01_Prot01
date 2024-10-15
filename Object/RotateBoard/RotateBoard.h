#pragma once

#include <vector>
#include <Vector2.h>
#include <Rect2.h>

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


private:
    void UpdateCourse();
};