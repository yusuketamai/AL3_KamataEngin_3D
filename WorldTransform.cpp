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

