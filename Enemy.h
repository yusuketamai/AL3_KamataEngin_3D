#pragma once
#include "KamataEngine.h"

class Player;

class Enemy {
public:
	// 初期化
	void Initialize(/*KamataEngine::WorldTransform* worldTransform,*/ KamataEngine::Model* model, 
		KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	//歩行の速さ
	static inline const float kWalkSpeed = 0.03f;

	//速度
	KamataEngine::Vector3 velocity_ = {};

	//最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.2f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 0.2f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionAngleTime = 0.5f;

	//経過時間
	float walkTimer_ = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	//AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision(const Player* player);

private:

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

};
