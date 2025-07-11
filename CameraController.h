#pragma once
#include "KamataEngine.h"
#include <vector>

//前方宣言
class Player;

class CameraController {
public:
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



private:
	// カメラ
	KamataEngine::Camera camera_;
	// 追従対象とカメラの座標の差(オフセット)
	KamataEngine::Vector3 targetOffset_; //	= Vector3(0, 0, -15.0f);
};
