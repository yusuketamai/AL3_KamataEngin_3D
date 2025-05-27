#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include <vector>

class GameScene {
public:
	// 初期化
	void Initialize();

	// デストラクタ
	~GameScene();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 3Dモデル
	KamataEngine::Model* modelBlock_ = nullptr;

	// 箱
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// カメラ
	KamataEngine::Camera camera_;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//スカイドーム3Dモデル
	KamataEngine::Model* modelSkydome_ = nullptr;

	//skyDome
	Skydome* skydome_ = nullptr;
};
