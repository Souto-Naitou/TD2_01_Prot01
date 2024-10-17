#include "EnemyPopSytem.h"
#include <random>

Enemy* EnemyPopSystem::Spawn(const Vector2& _start, const Vector2& _end)
{
    Enemy* pEnemy = new Enemy();
    Vector2 result = {};
    Vector2 range = _end - _start;

    result.x = static_cast<float>(rand() % static_cast<int>(range.x));
    result.y = static_cast<float>(rand() % static_cast<int>(range.y));
    pEnemy->SetWorldPosition(result);

    return pEnemy;
}

Enemy* EnemyPopSystem::Update(double _interval, const Vector2& _start, const Vector2& _end)
{
    Enemy* resultPtr = nullptr;

    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }

    if (timer_.GetNow() > _interval)
    {
        resultPtr = Spawn(_start, _end);
        timer_.Reset();
    }

    return resultPtr;
}