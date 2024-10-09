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

    inline void const SetCircle(std::vector<Vector2>* _circle) { pCircle_ = _circle; }

private:

    Vector2 position_ = {};
    std::vector<Vector2>* pCircle_ = {};
    uint32_t index_ = 0u;
    char keys[256] = {};
    char preKeys[256] = {};

    std::chrono::system_clock::time_point startTime_;
    std::unique_ptr<Easing> pEasingPlayerMove_;

};