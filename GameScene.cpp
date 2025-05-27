#include "GameScene.h"

using namespace KamataEngine;
using namespace MathUtility;

void GameScene::Initialize() {
	// 3dモデルの生成
	modelBlock_ = Model::Create();

	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);

	// skydomeの生成
	skydome_ = new Skydome();

	// skydomeの初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	//// 3dモデルの生成
	// modelSkydome_ = Model::Create();

	// カメラの初期化
	camera_.Initialize();

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 要素数
	const uint32_t kNumBlockVertical = 10;   // 縦
	const uint32_t kNumBlockHorizontal = 20; // 横

	// ブロック1個分の幅
	const float kBlockWidth = 2.0f;  // 縦
	const float kBlockHeight = 2.0f; // 横

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVertical);
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		// 1列の要素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if ((j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1)) {

				// ワールドトランスフォームの生成
				worldTransformBlocks_[i][j] = new WorldTransform();

				// ワールドトランスフォームの初期化
				worldTransformBlocks_[i][j]->Initialize();

				// x座標
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;

				// y座標
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			}
		}
	}
}

GameScene::~GameScene() {
	// モデルの解放
	delete modelBlock_;
	modelBlock_ = nullptr;

	// skydomeの解放
	delete modelSkydome_;
	modelSkydome_ = nullptr;

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
}

void GameScene::Update() {
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

	// skydomeのUPdate
	skydome_->Update();

	// デバックカメラの更新
	debugCamera_->Update();
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_D)) {
		isDebugCameraActive_ = !isDebugCameraActive_; // デバックカメラの有効無効を切り替え
	}
#endif

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
		camera_.UpdateMatrix();
	}
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

	// 3Dモデルの描画後処理
	Model::PostDraw();
}
