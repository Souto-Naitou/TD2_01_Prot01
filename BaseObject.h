#pragma once
#include <Vector2.h>

class BaseObject
{
public:
    Vector2 GetWorldPosition() const { return position_; };

protected:
    Vector2 position_;
};