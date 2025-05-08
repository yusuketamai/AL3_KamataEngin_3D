#include <Windows.h>
#include <KamataEngine.h>
#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	//エンジンの初期化
	KamataEngine::Initialize(L"GC2C_07_タマイ_ユウスケ_AL3");

	// DirectXCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// ゲームシーンのインスタンス処理
	GameScene* gameScene = new GameScene();

	//ゲームシーンのインスタンス処理
	gameScene->Initialize();

	//メインループ
	while (true) {
	//エンジンの更新
		if (KamataEngine::Update()) {

		break;
		}

		//ゲームシーンの更新
		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();

		// ゲームシーンの描画
		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();

	}
	//ゲームシーンの解放
	delete gameScene;

	//nullptrの代入
	gameScene = nullptr;

	//エンジンのメイン処理
	KamataEngine::Finalize();

	return 0;
}
