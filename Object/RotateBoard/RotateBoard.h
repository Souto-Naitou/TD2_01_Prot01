#pragma once

#include <vector>
#include <Vector2.h>
#include <Rect2.h>

class RotateBoard
{
public:

    RotateBoard() = default;
    ~RotateBoard() = default;

    void Initialize();
    void Update();
    void Draw();

    void SetVertices(const std::vector<Vector2>& _vertices) { parentVertices_ = _vertices; }

private:
    Rect2 rect_;
    std::vector<Vector2> parentVertices_;
    std::vector<Vector2> course_;
};