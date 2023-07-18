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
	// �A�E�g�v�b�g�p�f�[�^�̃��Z�b�g
	/*for (int y = 0; y < STAGE_DEPTH; y++)
	{
		std::vector<int> push;
		push.push_back(0);
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			m_stageFileData.push_back(push);
		}
	}*/

	// �t�@�C�����J��
	std::ifstream ifs(fileName);

	// �t�@�C�����ǂݍ��߂Ă��邩�`�F�b�N
	if (ifs.fail())
	{
		return false;
	}

	// �s�̕������ۑ����邽�߂̕ϐ�
	std::string line;
	int maxX = 0;
	int maxY = 0;

	// 1�s���}�b�v�f�[�^��ǂݍ���ł���
	for (int y = 0; getline(ifs, line); y++)
	{

		if (y >= STAGE_HEIGHT)
		{
			// �w�肳�ꂽ�ő�l�𒴂����̂ŁA���[�v���I����
			break;
		}

		// ������X�g���[����1�s�����
		std::istringstream iss(line);

		// �J���}��؂�̕������󂯎�邽�߂̕ϐ�
		std::string buf;

		// �s�̒��ɂ���J���}��؂�̃f�[�^�����ԂɎ󂯎��
		for (int x = 0; getline(iss, buf, ','); x++)
		{
			if (x >= STAGE_WIDTH)
			{
				// �w�肳�ꂽ�ő�l�𒴂����̂ŁA���[�v���I����
				break;
			}
			// �������int�ɕϊ�
			int num = stoi(buf);

			// �X�e�[�W�z��Ƀf�[�^������
			//a.push_back(num);
			
			m_stageData[y][x] = num;

			std::cout << num;

			/*DirectX::SimpleMath::Matrix fixedScale=*/

			//�L�����N�^�[�܂߃}�b�v�f�[�^�������ŏ�����Ă���
			/*m_pMapObjectList[(y * STAGE_WIDTH) + x]->SetPosition(Position{ x * 32.0f,y * 32.0f });

			//m_pMapObjectList[(y * STAGE_WIDTH) + x]->SetSize(Pixel{ 32,32 });

			m_pMapObjectList[(y * STAGE_WIDTH) + x]->SetTextureID(m_mapTextureID1);


			m_pMapObjectList[(y * STAGE_WIDTH) + x]->Initialize();*/
		}
		//m_stageFileData.push_back(a);
	}

	// �t�@�C�������
	ifs.close();

	// �t�@�C������f�[�^�̎擾����������
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
		// �������int�ɕϊ����Ċi�[
		while (std::getline(ss, cell, ','))
		{
			row.push_back(std::stoi(cell)); 
		}

		data.push_back(row);
	}

	file.close();
}

// 2������int�z���CSV�t�@�C���ɏ������ފ֐�
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