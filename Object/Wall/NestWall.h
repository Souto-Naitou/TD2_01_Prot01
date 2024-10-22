#pragma once

#include <Rect2.h>
#include "BaseObject.h"
#include "Collision/Collider.h"
#include <string>
#include "Collision/CollisionManager.h"

class NestWall : public BaseObject
{
public: /// 公開メソッド

    NestWall(std::string _ID);
    ~NestWall();

    void                Initialize();
    void                RunSetMask();
    void                Update();
    void                Draw();



    void                OnCollisionTrigger(const Collider* _collider);
    Collider*           GetCollider() { return &collider_; }


    void                SetRect(int _width, int _height, Vector2 _leftTop);


private: /// 非公開データ
    Rect2               rect_;
    Collider            collider_;
    bool                isDead_ = false;


private: /// 非公開メソッド
    void                DebugWindow();


private: /// 他オブジェクトのデータ
    CollisionManager* pCollisionManager_;
};