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

	float param = std::sin();
	flaot degree = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x=

	worldTransform_.UpdateMatrix();
	//あ

}

void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }
