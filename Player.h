#pragma once


#include <easing/Easing.h>
#include <Vector2.h>

#include <vector>
#include <chrono>
#include <memory>

class Player
{
public:

    void Initialize();
    void Update();
    void Draw();

private:
    char keys[256] = {};
    char preKeys[256] = {};

    std::vector<Vector2> circle_ = {};

    float radius_default = 100.0f;
    float radius_min = 20.0f;
    float radius_ = 0.0f;

    size_t resolution_ = 4u;
    Vector2 originPosition_ = {};

    std::chrono::system_clock::time_point startTime_;
    std::unique_ptr<Easing> pEasingDecreaseSize_;
    std::unique_ptr<Easing> pEasingIncreaseSize_;

};