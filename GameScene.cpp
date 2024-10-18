#include "GameScene.h"

#include "DefaultSettings.h"

#include <Novice.h>
#include "Player.h"
#include "Object/Enemy/Enemy.h"
#include "Object/Core/Core.h"
#include "ImGuiDebugManager/DebugManager.h"

#include <cmath>
#include <numbers>

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

GameScene::GameScene()
{
    pCollisionManager_ = CollisionManager::GetInstance();
    DebugManager::GetInstance()->SetComponent("GameScene", std::bind(&GameScene::DebugWindow, this));
}

GameScene::~GameScene()
{
    pCollisionManager_->ClearColliderList();
    DebugManager::GetInstance()->DeleteComponent("GameScene");
    delete pCore_; pCore_ = nullptr;
    delete pNestWallLeft_; pNestWallLeft_ = nullptr;

    for (Enemy* ptr : enemyList_)
    {
        delete ptr;
        ptr = nullptr;
    }
}

void GameScene::Initialize()
{
#ifdef _DEBUG
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif // _DEBUG

    pCollisionManager_->Initialize();

    pEnemyPopSystem_ = new EnemyPopSystem();

    pPlayer_ = new Player();
    pPlayer_->Initialize();

    pCore_ = new Core();
    pCore_->Initialize();

    uint32_t nestWallWidth = 40u;

    MakeWall(&pNestWallLeft_, "Left", nestWallWidth, DefaultSettings::kScreenHeight, { 0,0 });
    MakeWall(&pNestWallTop_, "Top", DefaultSettings::kScreenWidth, nestWallWidth, { 0,0 });
    MakeWall(&pNestWallRight_, "Right", nestWallWidth, DefaultSettings::kScreenHeight, { DefaultSettings::kScreenWidth - nestWallWidth, 0u });
    MakeWall(&pNestWallBottom_, "Bottom", DefaultSettings::kScreenWidth, nestWallWidth, { 0u,DefaultSettings::kScreenHeight - nestWallWidth });

    /// マスクの生成にアトリビュートを使用するためInitialize後に行う
    pPlayer_->RunSetMask();

    pCore_->RunSetMask();
    pNestWallLeft_->RunSetMask();
    pNestWallTop_->RunSetMask();
    pNestWallRight_->RunSetMask();
    pNestWallBottom_->RunSetMask();

    timer_.Start();
}

void GameScene::Update()
{
    /// タイマーが止まっていたらスタートする
    if (!timer_.GetIsStart())
    {
        timer_.Start();
    }


    /// 各オブジェクトの更新処理呼出
    pPlayer_->Update();
    pCore_->Update();
    pNestWallLeft_->Update();
    pNestWallTop_->Update();
    pNestWallRight_->Update();
    pNestWallBottom_->Update();
    for (Enemy* ptr : enemyList_) ptr->Update();

    pCollisionManager_->CheckAllCollision();


    /// PopSystem
    if (isPop_)
    {
        Enemy* ptrEnemy = pEnemyPopSystem_->Update(0.01, { 0,0 }, { DefaultSettings::kScreenWidth, DefaultSettings::kScreenHeight });
        if (ptrEnemy)
        {
            /// Enemyの初期化処理
            ptrEnemy->Initialize(enemyList_.size());
            ptrEnemy->SetTargetPosition(pPlayer_->GetWorldPosition());
            ptrEnemy->RunSetMask();
            ptrEnemy->SetEnableLighter(isEnableLighter_);
            ptrEnemy->SetBouncePower(Enemy::BounceTarget::Enemy, e2eBouncePower_);
            enemyList_.push_back(ptrEnemy);
        }
    }

    /// Delete処理ここから
    for (std::list<Enemy*>::iterator itr = enemyList_.begin(); itr != enemyList_.end();)
    {
        if (!(*itr)->GetIsDead())
        {
            itr++;
            continue;
        }
        delete *itr;
        itr = enemyList_.erase(itr);
    }
}

void GameScene::Draw()
{
    // 背景
    Novice::DrawBox(0, 0, DefaultSettings::kScreenWidth, DefaultSettings::kScreenHeight, 0.0f, 0x242424ff, kFillModeSolid);

    pNestWallLeft_->Draw();
    pPlayer_->Draw();
    pCore_->Draw();
    pNestWallLeft_->Draw();
    pNestWallTop_->Draw();
    pNestWallRight_->Draw();
    pNestWallBottom_->Draw();
    for (Enemy* ptr : enemyList_) ptr->Draw();

}

void GameScene::DebugWindow()
{
    if (ImGui::Checkbox("Enable Lag Reduction (beta)", &isEnableLighter_))
    {
        pPlayer_->SetEnableLighter(isEnableLighter_);
        pCore_->SetEnableLighter(isEnableLighter_);
        for (Enemy* enemy : enemyList_)
        {
            enemy->SetEnableLighter(isEnableLighter_);
        }
    }
    ImGui::Checkbox("EnemyPop", &isPop_);

    ImGui::Text("Enemy to Enemy Bounce Power");
    if (ImGui::DragFloat("Power", &e2eBouncePower_))
    {
        for (Enemy* enemy : enemyList_)
        {
            enemy->SetBouncePower(Enemy::BounceTarget::Enemy, e2eBouncePower_);
        }
    }

    if (ImGui::SmallButton("Delete All Enemies"))
    {
        for (Enemy* enemy : enemyList_)
        {
            enemy->SetIsDead(true);
        }
    }
}

void GameScene::MakeWall(NestWall** _nestWall, std::string _id, int _width, int _height, Vector2 _origin)
{
    *_nestWall = new NestWall(_id);
    (*_nestWall)->SetRect(_width, _height, _origin);
    (*_nestWall)->Initialize();
}
