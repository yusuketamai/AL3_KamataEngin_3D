#include "Enemy.h"
#include <cassert>
#include <numbers>
#include<math.h>
#include<cmath>

using namespace KamataEngine;

using namespace MathUtility;

void Enemy::Initialize(/*KamataEngine::WorldTransform* worldTransform,*/ KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	// 移動
	worldTransform_.translation_ += velocity_;

	//タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	//回転アニメーション
	worldTransform_.rotation_.x = std::sin(walkTimer_ * 5.0f);

	float param = std::sin((2 * PI) * walkTimer_ / kWalkMotionAngleTime);
	float degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = std::sin(degree);

	worldTransform_.UpdateMatrix();
	

}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }

KamataEngine::Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player; }
