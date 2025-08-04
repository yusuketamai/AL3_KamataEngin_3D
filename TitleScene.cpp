#include "TitleScene.h"

#include <numbers>

using namespace KamataEngine;

void TitleScene::Initialize() {
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");

	camera_.Initialize();

	worldTransform_.Initialize();
	worldTransform_.scale_ = {2, 2, 2};
	worldTransform_.translation_ = {0, 8, 0};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10, 10, 10};
	worldTransformPlayer_.translation_ = {0, -8, 0};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
}

TitleScene::~TitleScene() {

	delete model_;
	delete modelPlayer_;
}

void TitleScene::Update() {

	worldTransform_.MakeAfinneMatrix();

	worldTransform_.TransferMatrix();

	rotate += 0.1f;

	worldTransformPlayer_.rotation_.y = sin(rotate) + std::numbers::pi_v<float>;

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
}