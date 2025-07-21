#pragma once
#include "KamataEngine.h"
#include "GameScene.h"



class MapChipField;

class Player {
public:

	// マップとの当たり判定情報
	struct CollisionMapInfo {

		//天井衝突フラグ
		bool ceiling = false;
		// 着地フラグ
		bool landing = false;
		//壁接触フラグ
		bool hitWall = false;

		KamataEngine::Vector3 move;
	};

	KamataEngine::Vector3 velocity_ = {};

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	//マップ衝突判定
	void CheckMapCollision(CollisionMapInfo& info);

	// マップ衝突判定_上
	void CheckMapCollisionUp(CollisionMapInfo& info);

	// 更新
	void Update();

	// 移動入力
	void InputMove();

	void AnimeteTurn();


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

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) {

		mapChipField_ = mapChipField;
	}

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 角
	enum Corner {

		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数

	};

	//指定した角の座標計算
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);


	// 隙間
	static inline const float kBlank = 0.1f;
		
	void CheckmapMove(const CollisionMapInfo& info);

private:

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;
};
