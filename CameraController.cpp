#include "CameraController.h"
#include"Player.h"

using namespace KamataEngine;
using namespace MathUtility;

void CameraController::Initialize() {

	camera_.Initialize();
	targetOffset_ = Vector3(0, 0, -15.0f);
}

void CameraController::UPdate() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
	//行列を更新する
	camera_.UpdateMatrix();
}
void CameraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	//追従対象とオフセットからカメラの座標を計算
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}