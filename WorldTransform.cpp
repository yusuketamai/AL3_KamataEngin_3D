#include "3d\WorldTransform.h"
#include <math\MathUtility.h>
#include<cmath>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;


void WorldTransform::MakeAfinneMatrix() {
	// アフィン変換行列の作成
	Matrix4x4 scaleMatrix_ = MakeScaleMatrix(scale_);                                                         // スケール
	Matrix4x4 rotationMatrixX_ = MakeRotateXMatrix(rotation_.x);                                              // 回転X
	Matrix4x4 rotationMatrixY_ = MakeRotateYMatrix(rotation_.y);                                              // 回転Y
	Matrix4x4 rotationMatrixZ_ = MakeRotateZMatrix(rotation_.z);                                              // 回転Z
	Matrix4x4 translationMatrix_ = MakeTranslateMatrix(translation_);                                         // 平行移動
	matWorld_ = scaleMatrix_ * (rotationMatrixX_ * rotationMatrixY_ * rotationMatrixZ_) * translationMatrix_; // アフィン変換行列;
}

void WorldTransform::UpdateMatrix(){

	MakeAfinneMatrix();
	TransferMatrix();

}

float WorldTransform::EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

//衝突判定
bool WorldTransform::IsCollision(const AABB& aabb1, const AABB& aabb2) { 
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // x軸
	       (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // y軸
	       (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);   // z軸