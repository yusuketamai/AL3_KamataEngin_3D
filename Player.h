#pragma once
#include "KamataEngine.h"

class Player {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera,const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	Vector3 velocity_ = {};

	static inline const float kAcceleration = {};

	static inline const float kAttenuation = {};

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	

};
