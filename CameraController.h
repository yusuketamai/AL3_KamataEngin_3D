#pragma once
#include "KamataEngine.h"


//前方宣言
class Player;

class CameraController {
public:

	// 矩形
	struct Rect {

		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	///< summary>
	/// 初期化
	///  </summary>
	void Initialize();

	///< summary>
	/// 初期化
	///  </summary>
	void UPdate();

	Player* target_ = nullptr;

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	const KamataEngine::Camera& GetViewProjection() const { return camera_; }

	

	//カメラ制御
	Rect movableArea_ = {0, 100, 0, 100};

	void SetMovableArea(Rect area) {
		movableArea_ = area;
	}

	//カメラの目標座標
	KamataEngine::Vector3 targetPosition_; 

	//座標保管割合
	static inline const float kInterpolationRate = 0.2f;

	//速度掛け率
	static inline const float kVelocityBias = 15;

	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};


private:
	// カメラ
	KamataEngine::Camera camera_;
	// 追従対象とカメラの座標の差(オフセット)
	KamataEngine::Vector3 targetOffset_ ;
};
