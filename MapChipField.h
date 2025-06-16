#pragma once
#include "KamataEngine.h"


// マップチップタイプ
enum class MapChipType {
	kBlank, // 空白
	kBlock  // ブロック
};

struct MapChipData {

	std::vector<std::vector<MapChipType>> data;
};

class MapChipField 
{
public:
	// ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtalcal = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);



	MapChipType GetmapChiptypeByIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChippositionByIndex(uint32_t xIndex, uint32_t yIndex);

};
