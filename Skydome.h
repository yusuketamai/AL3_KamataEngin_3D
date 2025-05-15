#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;


class Skydome {
public:
	// 初期化
	void Initialize();

	// デストラクタ
	~Skydome();

	// 更新
	void Update();

	// 描画
	void Draw();

private:

	KamataEngine::WorldTransform wrldTransform_;

	Model* model_ = nullptr;



};
