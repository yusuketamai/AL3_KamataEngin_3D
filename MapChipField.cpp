#include "MapChipField.h"
#include <map>
#include<fstream>
#include<sstream>

namespace {

std::map<std::string, MapChipType> mapChipTable = {

    {"0", MapChipType::kBlank},
    {"0", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {

	// マップチップデータをリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtalcal);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	ResetMapChipData();

	//ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//マップチップでCSV
	std::stringstream mapChipCsv;
	//ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	//ファイルを閉じる
	file.close();

	//CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtalcal; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		//一行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
		
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}

		}
	}

}

MapChipType MapChipField::GetmapChiptypeByIndex(uint32_t xIndex, uint32_t yIndex) {

	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {

		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtalcal - 1 < yIndex) {

		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex]; 

}

KamataEngine::Vector3 MapChipField::GetMapChippositionByIndex(uint32_t xIndex, uint32_t yIndex) { 
	
	return KamataEngine::Vector3(kBlockWidth*xIndex,
		kBlockHeight*(kNumBlockVirtalcal-1-yIndex),0);

}
