#pragma once
#include "KamataEngine.h"

class Player {
public:
	KamataEngine::Vector3 velocity_ = {};

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	static inline const float kAcceleration = 0.3f;

	static inline const float kAttenuation = 0.5f;

	static inline const float kLimitRunSpeed = 0.3f;

	enum class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	//接地状態フラグ
	bool onGround_ = true;

	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.1f;

	// 最大落下速度(下方向)
	static inline const float kLimutFallSpeed = 0.5f;

	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.5f;

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;
};
