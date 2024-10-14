#pragma once

#include <Rect2.h>
#include "BaseObject.h"
#include "Collision/Collider.h"
#include <string>

class NestWall : public BaseObject
{
public:

    NestWall(std::string _ID);
    ~NestWall();

    void Initialize();
    void Update();
    void Draw();

    Collider* GetCollider() { return &collider_; }
    void SetRect(int _width, int _height, Vector2 _leftTop);

private:
    std::string id_;
    Rect2 rect_;
    Collider collider_;

    void DebugWindow();
};