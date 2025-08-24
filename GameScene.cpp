#include "GameScene.h"
#include <algorithm>
#include <cassert>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;

void GameScene::Initialize() {
	// 3dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("cube", true);

	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	modelParticles_ = Model::CreateFromOBJ("deathParticle", true);

	GenerateBlocks();

	// skydomeの生成
	skydome_ = new Skydome();

	// skydomeの初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	// カメラの初期化
	camera_.Initialize();

	////titleの初期化
	// titleScene_->Initialize();

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5, 18);

	// Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(24,18);

	// playerの生成
	player_ = new Player();

	// playerの初期化
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	// カメラコントローラーの生成
	cameraController_ = new CameraController;

	cameraController_->Initialize();

	cameraController_->SetTarget(player_);

	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	// Enemyの生成
	for (uint32_t i = 0; i < 5; i++) {
		// enemy の生成
		Enemy* newEnemy = new Enemy();
		// enemyの初期化
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(20 + i, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);

		enemys_.push_back(newEnemy);
	}

	// 仮の生成処理、後で消す
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelParticles_, &camera_, playerPosition);

	// フェードインから開始
	phase_ = Phase::kFadeIn;

	// フェード
	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

GameScene::~GameScene() {
	// モデルの解放
	delete modelBlock_;
	modelBlock_ = nullptr;

	// skydomeの解放
	delete modelSkydome_;
	modelSkydome_ = nullptr;

	delete skydome_;
	skydome_ = nullptr;

	// モデルplayerの開放
	delete modelPlayer_;
	modelPlayer_ = nullptr;

	// playerの開放
	delete player_;
	player_ = nullptr;

	// enemyの解放
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}

	// deathParticles_の解放
	delete deathParticles_;
	deathParticles_ = nullptr;

	// enemy_ = nullptr;

	// マップチップフィールドの解放
	delete mapChipField_;

	// 箱の解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// デバックカメラの解放
	delete debugCamera_;
	debugCamera_ = nullptr;

	// フェード
	delete fade_;
}

void GameScene::Update() {

	switch (phase_) {
	case Phase::kDeath:

		// skydomeのUPdate
		skydome_->Update();

		// enemyのUpdate
		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		// deathParticles_の更新
		if (deathParticles_) {
			deathParticles_->Update();
		}

		// カメラの更新
		if (isDebugCameraActive_) {
			// デバックカメラの更新
			debugCamera_->Update();

			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロダクションの転送
			camera_.TransferMatrix();
		} else {
			// ビュープロダクション行列の更新と転送
			camera_.TransferMatrix();

			camera_.matView = cameraController_->GetViewProjection().matView;
			camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// ブロックの穴あきを許容
				if (!worldTransformBlock) {
					continue; // nullチェック
				}

				// アフィン変換
				worldTransformBlock->MakeAfinneMatrix();

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		if (deathParticles_ != nullptr && deathParticles_->IsFinished()) {
			//フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		break;
	case Phase::kPlay:

		// skydomeのUPdate
		skydome_->Update();

		// playerのUPdate
		player_->Update();

		// デバックカメラの更新
		debugCamera_->Update();

		// カメラコントローラーの更新
		cameraController_->UPdate();

		// enemyのUpdate
		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// ブロックの穴あきを許容
				if (!worldTransformBlock) {
					continue; // nullチェック
				}

				// アフィン変換
				worldTransformBlock->MakeAfinneMatrix();

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();

		// カメラの更新
		if (isDebugCameraActive_) {
			// デバックカメラの更新
			debugCamera_->Update();

			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロダクションの転送
			camera_.TransferMatrix();
		} else {
			// ビュープロダクション行列の更新と転送
			camera_.TransferMatrix();

			camera_.matView = cameraController_->GetViewProjection().matView;
			camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		}

		ChangePhase();
		break;

	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		// skydomeのUPdate
		skydome_->Update();

		// playerのUPdate
		player_->Update();

		// デバックカメラの更新
		debugCamera_->Update();

		// カメラコントローラーの更新
		cameraController_->UPdate();

		// enemyのUpdate
		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// ブロックの穴あきを許容
				if (!worldTransformBlock) {
					continue; // nullチェック
				}

				// アフィン変換
				worldTransformBlock->MakeAfinneMatrix();

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();

		// カメラの更新
		if (isDebugCameraActive_) {
			// デバックカメラの更新
			debugCamera_->Update();

			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロダクションの転送
			camera_.TransferMatrix();
		} else {
			// ビュープロダクション行列の更新と転送
			camera_.TransferMatrix();

			camera_.matView = cameraController_->GetViewProjection().matView;
			camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		}

		ChangePhase();
		break;

	case Phase::kFadeOut:
		// フェード
		fade_->Update();

		// skydomeのUPdate
		skydome_->Update();

		// playerのUPdate
		player_->Update();

		// デバックカメラの更新
		debugCamera_->Update();

		// カメラコントローラーの更新
		cameraController_->UPdate();

		// enemyのUpdate
		for (Enemy* enemy : enemys_) {
			enemy->Update();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// ブロックの穴あきを許容
				if (!worldTransformBlock) {
					continue; // nullチェック
				}

				// アフィン変換
				worldTransformBlock->MakeAfinneMatrix();

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();

		// カメラの更新
		if (isDebugCameraActive_) {
			// デバックカメラの更新
			debugCamera_->Update();

			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロダクションの転送
			camera_.TransferMatrix();
		} else {
			// ビュープロダクション行列の更新と転送
			camera_.TransferMatrix();

			camera_.matView = cameraController_->GetViewProjection().matView;
			camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		}

		ChangePhase();
		break;
	}



#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_D)) {
		isDebugCameraActive_ = !isDebugCameraActive_; // デバックカメラの有効無効を切り替え
	}
#endif
}

void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 箱の描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// ブロックの穴あきを許容
			if (!worldTransformBlock) {
				continue; // nullチェック
			}
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	skydome_->Draw();

	//自キャラの描画
	//if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn) {
		player_->Draw();
	//}
	

	// enemyの描画
	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}

	// deathParticles_の更新
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// 3Dモデルの描画後処理
	Model::PostDraw();

	// フェード
	fade_->Draw();
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			if (mapChipField_->GetmapChiptypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと散弾全ての当たり判定
	for (Enemy* enemy : enemys_) {
		// 散弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (aabb1.IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時間数を呼び出す
			player_->OnCollision(enemy);
			// 敵弾の衝突時のコールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}
}
//フェーズの切り替え
void GameScene::ChangePhase() {

	switch (phase_) {
	case GameScene::Phase::kPlay:

		if (player_->isDead_) {

			// 死亡演出
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル生成
			deathParticles_ = new DeathParticles();

			// 3Dモデルの設定
			modelParticles_ = Model::CreateFromOBJ("deathParticle", true);

			deathParticles_->Initialize(modelParticles_, &camera_, deathParticlesPosition);
			break;
		}
		break;
	case GameScene::Phase::kDeath:
		//デスパーティクル処理
		if (deathParticles_->IsFinished()) {
		//フェードアウト開始
			phase_ = Phase::kFadeOut;
		fade_->Start(Fade::Status::FadeOut, 1.0f);
		
		}
		break;
	case GameScene::Phase::kFadeIn:
		if (fade_->IsFinished()) {
		//ゲームプレイへ切り替え
			phase_ = Phase::kPlay;
		}
		break;
	case GameScene::Phase::kFadeOut:
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}