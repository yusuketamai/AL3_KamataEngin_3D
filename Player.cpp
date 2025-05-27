#include "Player.h"
#include <cassert>

void Player::Initialize(KamataEngine::Model* model,  KamataEngine::Camera* camera) {
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
}

void Player::Update() { worldTransform_.UpdateMatrix(); }

void Player::Draw() { model_->Draw(worldTransform_, *camera_ ); }
