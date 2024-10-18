#pragma once

#include "BaseObject.h"
#include "Collision/Collider.h"
#include <Vector2.h>

class Enemy : public BaseObject
{
public:

    enum class BounceTarget
    {
        Enemy,
        Player
    };

    Enemy() = default;
    ~Enemy();

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="idx">数多のEnemy内で一意の数字 (リストのサイズを渡すと楽)</param>
    void                            Initialize(size_t idx);


    /// <summary>
    /// 更新
    /// </summary>
    void                            Update();


    /// <summary>
    /// 描画
    /// </summary>
    void                            Draw();


    /// <summary>
    /// コライダーを取得
    /// </summary>
    /// <returns>コライダーのポインタ</returns>
    inline      const Collider*     GetCollider()
    {
        return &collider_;
    }


    void                            RunSetMask();


    /// <summary>
    /// ターゲットにする座標を指定
    /// </summary>
    /// <param name="_target">座標</param>
    inline      void                SetTargetPosition(const Vector2& _target)
    {
        positionTarget_ = _target;
    }


    /// <summary>
    /// 衝突時コールバック関数
    /// </summary>
    /// <param name="_other">衝突した相手のコライダー</param>
    void                            OnCollision(const Collider* _other);


    /// <summary>
    /// ラグ軽減を行うか
    /// </summary>
    /// <param name="_flag">フラグ</param>
    void                            SetEnableLighter(bool _flag);


    /// <summary>
    /// 反発力を設定
    /// </summary>
    /// <param name="_target">誰とあたったときか</param>
    /// <param name="_power">反発力</param>
    void                            SetBouncePower(BounceTarget _target, float _power);


    /// <summary>
    /// 死亡フラグを設定
    /// </summary>
    /// <param name="_flag">フラグ</param>
    void                            SetIsDead(bool _flag) { isDead_ = _flag; }

private: /// 非公開データ
    Collider                        collider_                   = {};           // コライダー
    Vector2                         vertices_[3]                = {};           // 三角形の頂点

    std::string                     idx_                        = {};           // index (Enemy内の一意の数字)

    Vector2                         positionTarget_             = {};           // ターゲットのベクトル (原点中心)
    Vector2                         distanceToTarget            = {};           // ターゲットまでのベクトル
    Vector2                         ellipseAB_                  = {};           // 楕円を構成するAとB
    float                           rotation_                   = 0.0f;         // 回転角
    float                           radius_                     = 0.0f;         // 半径
    float                           moveSpeed_                  = 0.0f;         // 移動スピード
    float                           collisionCooldown_          = 0.0f;         // 衝突判定クールダウン
    const float                     collisionCooldownDuration_  = 0.5f;         // 衝突判定クールダウン感覚

    char*                           keys_                       = nullptr;      // 今フレームのキー情報
    char*                           preKeys_                    = nullptr;      // 前フレームのキー情報

    float                           bounceSpeed_                = 5.0f;         // 衝突時のぶっとび速度
    bool                            isBouncing_                 = false;        // 跳ね返りフラグ

    bool                            hasCollided_                = false;        // 衝突フラグ
    int32_t                         outScreenPadding_           = 0ui32;        // スクリーン外判定を行う範囲の余白
    bool                            isOutOfScreen_              = false;        // スクリーン外かどうか
    float                           bouncePower_enemy_          = 2.0f;         // Enemy同士の反発力

private: /// シングルトン インスタンスのポインタ
    CollisionManager* pCollisionManager_;

private: /// 非公開メソッド
    void DebugWindow();
};