#pragma once

#include "Collider.h"
#include <vector>
#include <string>
#include <utility>

class CollisionManager
{
public:
    CollisionManager(const CollisionManager&) = delete;
    CollisionManager(const CollisionManager&&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&&) = delete;

    static CollisionManager* GetInstance() { static CollisionManager instance; return &instance; }

    void Initialize();
    void CheckAllCollision();
    void RegisterCollider(Collider* _collider);
    void ClearColliderList();
    uint32_t GetNewAttribute(std::string _id);
    template <typename... Args>
    uint32_t* GetNewMask(std::string _id, Args... _ignoreNames)
    {
        uint32_t result = 0;
        for (auto& attributePair : attributeList_)
        {
            if (_id.compare(attributePair.first) == 0)
            {
                result = ~attributePair.second;
                break;
            }
        }
        for (std::string name : std::initializer_list<std::string>{ _ignoreNames... })
        {
            for (auto& attributePair : attributeList_)
            {
                if (name.compare(attributePair.first) == 0)
                {
                    result ^= attributePair.second;
                }
            }
        }

        uint32_t* resultPtr = nullptr;
        bool isCompare = false;
        /// 配列の中身を変更
        for (auto& maskPair : maskList_)
        {
            if (maskPair.first.compare(_id) == 0)
            {
                maskPair.second = result;
                resultPtr = &maskPair.second;
                isCompare = true;
            }
        }
        if (!isCompare) // マスクリストに登録されていなかったら
        {
            maskList_.push_back({ _id, result });
            resultPtr = &maskList_.back().second;
        }

        return resultPtr;
    }


private:
    CollisionManager() = default;

    std::vector<Collider*> colliders_;
    std::vector<std::pair<std::string, std::string>> collisionNames_;
    std::vector<std::pair<std::string, uint32_t>> attributeList_;
    std::list<std::pair<std::string, uint32_t>> maskList_;

    void DebugWindow();
    void CheckCollisionPair(Collider* _colA, Collider* _colB);
    void ProjectShapeOnAxis(std::vector<Vector2>* _v, const Vector2& _axis, float& _min, float& _max);
};