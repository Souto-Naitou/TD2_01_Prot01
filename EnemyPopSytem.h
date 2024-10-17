#pragma once

#include "Object/Enemy/Enemy.h"
#include "BasePopSytem.h"
#include <Vector2.h>

class EnemyPopSystem : public BasePopSystem
{
public:
    Enemy* Spawn(const Vector2& _start, const Vector2& _end);

    Enemy* Update(double _interval, const Vector2& _start, const Vector2& _end);

private:

};