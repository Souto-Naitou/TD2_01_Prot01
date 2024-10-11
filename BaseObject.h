#pragma once
#include <Vector2.h>

class BaseObject
{
public:
    virtual void Initialize() {};
    virtual void Update() {};
    virtual void Draw() {};

    Vector2 GetWorldPosition() const { return position_; };

protected:
    Vector2 position_;
};