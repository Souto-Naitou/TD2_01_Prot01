#include <Novice.h>

#include <easing/EasingManager/EasingManager.h>
#include "GameScene.h"
#include "ImGuiDebugManager/DebugManager.h"
#include "InputCenter.h"
#include <stdlib.h>
#include <time.h>
#include "DefaultSettings.h"

const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, DefaultSettings::kScreenWidth, DefaultSettings::kScreenHeight);

	srand(static_cast<uint32_t>(time(nullptr)));

	char* pKeys, *pPreKeys;
	pKeys = pPreKeys = nullptr;

	DebugManager* pDebugManager = DebugManager::GetInstance();
	pDebugManager->ChangeFont();

	InputCenter* pInput = InputCenter::GetInstance();
	pKeys = pInput->GetKeyPtr();
	pPreKeys = pInput->GetPreKeyPtr();

	GameScene* pGameScene_ = new GameScene;
	pGameScene_->Initialize();

	EasingManager* pEasingManager = EasingManager::GetInstance();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		///
		/// ↓更新処理ここから
		///

		pInput->UpdateState();

		pDebugManager->DrawUI();

		pEasingManager->DrawUI();

		pGameScene_->Update();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		pGameScene_->Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (pKeys[DIK_ESCAPE] == 0 && pPreKeys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete pGameScene_;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
