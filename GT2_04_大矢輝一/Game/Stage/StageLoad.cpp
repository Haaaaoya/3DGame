#include "pch.h"
#include "StageLoad.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <SimpleMath.h>

StageLoad::StageLoad():
	m_stagePosition{0,0},
	m_maxX(0),
	m_maxY(0)
{
}

StageLoad::~StageLoad()
{
}

bool StageLoad::LoadStageData(const char* fileName)
{
	// アウトプット用データのリセット
	/*for (int y = 0; y < STAGE_DEPTH; y++)
	{
		std::vector<int> push;
		push.push_back(0);
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			m_stageFileData.push_back(push);
		}
	}*/

	// ファイルを開く
	std::ifstream ifs(fileName);

	// ファイルが読み込めているかチェック
	if (ifs.fail())
	{
		return false;
	}

	// 行の文字列を保存するための変数
	std::string line;
	int maxX = 0;
	int maxY = 0;

	// 1行ずつマップデータを読み込んでいく
	for (int y = 0; getline(ifs, line); y++)
	{

		if (y >= STAGE_HEIGHT)
		{
			// 指定された最大値を超えたので、ループを終える
			break;
		}

		// 文字列ストリームに1行入れる
		std::istringstream iss(line);

		// カンマ区切りの文字を受け取るための変数
		std::string buf;

		// 行の中にあるカンマ区切りのデータを順番に受け取る
		for (int x = 0; getline(iss, buf, ','); x++)
		{
			if (x >= STAGE_WIDTH)
			{
				// 指定された最大値を超えたので、ループを終える
				break;
			}
			// 文字列をintに変換
			int num = stoi(buf);

			// ステージ配列にデータを入れる
			//a.push_back(num);
			
			m_stageData[y][x] = num;

			std::cout << num;

			/*DirectX::SimpleMath::Matrix fixedScale=*/

			//キャラクター含めマップデータがここで書かれている
			/*m_pMapObjectList[(y * STAGE_WIDTH) + x]->SetPosition(Position{ x * 32.0f,y * 32.0f });

			//m_pMapObjectList[(y * STAGE_WIDTH) + x]->SetSize(Pixel{ 32,32 });

			m_pMapObjectList[(y * STAGE_WIDTH) + x]->SetTextureID(m_mapTextureID1);


			m_pMapObjectList[(y * STAGE_WIDTH) + x]->Initialize();*/
		}
		//m_stageFileData.push_back(a);
	}

	// ファイルを閉じる
	ifs.close();

	// ファイルからデータの取得が完了した
	return true;
}

void StageLoad::readCSV(const char* filename, std::vector<std::vector<int>>& data)
{
	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line))
	{
		std::vector<int> row;
		std::stringstream ss(line);
		std::string cell;
		// 文字列をintに変換して格納
		while (std::getline(ss, cell, ','))
		{
			row.push_back(std::stoi(cell)); 
		}

		data.push_back(row);
	}

	file.close();
}

// 2次元のint配列をCSVファイルに書き込む関数
void StageLoad::writeCSV(const char* filename, const std::vector<std::vector<int>>& data)
{
	std::ofstream file(filename);

	for (std::vector<int> row : data)
	{
		for (int cell : row)
		{
			file << cell << ",";
		}
		file << "\n";
	}

	file.close();
}

void StageLoad::Update()
{
	//LoadStageData("Resources/StageData/StageData1.csv");
}

DirectX::SimpleMath::Vector2 StageLoad::GetStagePosition()
{
	return m_stagePosition;
}