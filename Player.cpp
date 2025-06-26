#include "Player.h"
#include <cassert>
#include<numbers>
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

void Player::Update() { 

	worldTransform_.UpdateMatrix(); 

	//移動入力
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || 
		Input::GetInstance()->PushKey(DIK_LEFT) ){

		//左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		
			acceleration.x += kAcceleration;

		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		
			acceleration.x -= kAcceleration;
		}

		//加速/原則
		velocity_ += acceleration;

	}


	//移動
	worldTransform_.translation_ += velocity_;

	//行列更新

}

void Player::Draw() { model_->Draw(worldTransform_, *camera_ ); }
