#pragma once
#include <Vector2.h>
#include <string>

class BaseObject
{
public:
    virtual void Initialize() {};
    virtual void Update() {};
    virtual void Draw() {};
    virtual void DebugWindow();

    Vector2 GetWorldPosition() const { return position_; };
    uint32_t GetHP() const { return hp_; }

protected:
    std::string objectID_;
    Vector2 position_;
    int32_t hp_;
};