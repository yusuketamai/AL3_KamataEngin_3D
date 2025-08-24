#pragma once
#include <KamataEngine.h>
#include "Fade.h"

class TitleScene {
public:
	//シーンのフェーズ
	enum class Phase {
		kFadeIn,//フェードイン
		kMain,//メイン部
		kFadeOut,//フェードアウト
	};

	//デストラクタ
	~TitleScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	//回転
	float rotate = 0.0f;

	//終了フラグ
	bool finished_ = false;

	//フェード
	Fade* fade_ = nullptr;

	//現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

};