#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include <algorithm>
#include <cassert>
#include <numbers>
using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0;
}

// 移動入力
void Player::InputMove() {

	// 接地状態
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

				acceleration.x += kAcceleration;

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
				}

				acceleration.x -= kAcceleration;
			}
			// 加速/原則
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {

			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {

			// ジャンプ速度
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
		// 空中
	} else {

		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);

		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimutFallSpeed);
	}
}

void Player::AnimeteTurn() {
	// 旋回制御
	if (turnTimer_ > 0.0f) {

		turnTimer_ = 1.0f / 60.0f;

		// 左右の自キャラの角度テーブル
		float destinationRotationYTable[] = {

		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,

		};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = worldTransform_.EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_);
	}
}

// ②
void Player::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUp(info);
	// CheckMapCollisionDown(info);
	// CheckMapCollisionRight(info);
	// CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) {

	// 上昇あり？
	if (info.move.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定
	bool hit = false;
	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetmapChiptypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetmapChiptypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, kHeight / 2.0f, 0));
		// 名込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}
void Player::Update() {

	// ①移動入力
	InputMove();

	// ②移動量を加味して衝突判定する

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	//collisionMapInfo.move = velocity_;

	CheckmapMove(collisionMapInfo);

	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	AnimeteTurn();

	// 移動
	worldTransform_.translation_ += velocity_;

	// 行列更新
	worldTransform_.UpdateMatrix();

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中?
	if (velocity_.y < 0) {

		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {

		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}

	} else {

		// 着地
		if (landing) {
			// めり込み
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAcceleration);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); }

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner]{
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0},
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::CheckmapMove(const CollisionMapInfo& info) 
{

	//移動
	worldTransform_.translation_ += info.move;

}
