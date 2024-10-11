#pragma once


#include <easing/Easing.h>
#include <Vector2.h>
#include "BaseObject.h"

#include <vector>
#include <chrono>
#include <memory>

class Player : public BaseObject
{
public:
    Player();
    ~Player();

    void Initialize();
    void Update();
    void Draw();

private:
    char keys[256] = {};
    char preKeys[256] = {};

    std::vector<Vector2> circle_ = {};

    float radius_default_ = 100.0f;
    float radius_min_ = 20.0f;
    float radius_current_ = 0.0f;
    float radius_timeRelease_ = 0.0f;

    size_t resolution_ = 4u;

    std::chrono::system_clock::time_point startTime_;
    std::unique_ptr<Easing> pEasingBoxResize_;
    std::unique_ptr<Easing> pEasingBoxTemp_;


private:
    void DebugWindow();
};