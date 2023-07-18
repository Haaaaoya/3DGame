#pragma once
#include <vector>
#include <SimpleMath.h>

class StageLoad
{public:
	StageLoad();
	~StageLoad();

	bool LoadStageData(const char* fileName);
	void readCSV(const char* filename, std::vector<std::vector<int>>& data);
	void writeCSV(const char* filename, const std::vector<std::vector<int>>& data);

	void Update();

	DirectX::SimpleMath::Vector2 GetStagePosition();
	int GetStageData(int y, int x)
	{
		return m_stageData[y][x];
	}
private:
	static const int STAGE_WIDTH = 8;
	static const int STAGE_HEIGHT = 8;

	//std::vector<std::vector<int>> m_stageFileData;

	int m_stageData[8][8];
	int m_maxX;
	int m_maxY;

	DirectX::SimpleMath::Vector2 m_stagePosition;
};