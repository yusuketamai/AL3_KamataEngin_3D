#pragma once
#include "CameraController.h"
#include "Collision.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Skydome.h"
#include "player.h"
#include <vector>
#include "TitleScene.h"
#include "Fade.h"

class GameScene {
public:

	//gameのフェーズ
	enum class Phase {
		kFadeIn,//フェードイン
		kPlay,//ゲームプレイ
		kDeath,//デス演出
		kFadeOut, // フェードアウト
	};

	//ゲームの現在フェーズ(変数)
	Phase phase_;

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

	// マップチップフィールド
	MapChipField* mapChipField_;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// スカイドーム3Dモデル
	KamataEngine::Model* modelSkydome_ = nullptr;

	// skyDome
	Skydome* skydome_ = nullptr;

	// プレイヤー3Dモデル
	KamataEngine::Model* modelPlayer_ = nullptr;

	Player* player_ = nullptr;

	// カメラコントローラー3Dモデル
	KamataEngine::Model* modelcameraController_ = nullptr;

	// CameraController
	CameraController* cameraController_ = nullptr;

	void GenerateBlocks();

	// Enemy3Dモデル
	KamataEngine::Model* modelEnemy_ = nullptr;

	// 削除
	// Enemy* enemy_ = nullptr;

	// 代わりに...
	std::list<Enemy*> enemys_;

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	// パーティクル3Dモデル
	KamataEngine::Model* modelParticles_ = nullptr;

	DeathParticles* deathParticles_ = nullptr;

	void ChangePhase();

	//終了フラグ
	bool finished_ = false;

	//デスフラグのゲッター
	bool IsFinished() const { return finished_; }

	// フェード
	Fade* fade_ = nullptr;

};
