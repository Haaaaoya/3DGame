#include "pch.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager():
	m_position(DirectX::SimpleMath::Vector3::Zero)
{
}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Initialize(int stageNumber)
{
	m_stageLoad = std::make_unique<StageLoad>();

	//ステージデータを入れていく
	std::vector<const char*> fileName{
		{"Resources/StageData/StageData1.csv"},
		{"Resources/StageData/StageData2.csv"},
		{"Resources/StageData/StageData3.csv"},
		{"Resources/StageData/StageData4.csv"},
		{"Resources/StageData/StageData5.csv"},
		{"Resources/StageData/StageCreate.csv"},
	};
	//引数をもとにデータを読み込む
	m_stageLoad->LoadStageData(fileName[stageNumber - 1]);
	//タイルクラスとフロアクラスに
	//カメラクラスとステージロードクラスのポインタを送る
	m_normalTile = std::make_unique<NormalTile>();
	m_normalTile->SetPlaySceneCamera(m_playSceneCamera);
	m_normalTile->SetStageLoad(m_stageLoad.get());
	//初期化
	m_normalTile->Initialize();
	m_floor = std::make_unique<Floor>();
	m_floor->SetPlaySceneCamera(m_playSceneCamera);
	m_floor->SetStageLoad(m_stageLoad.get());
	//初期化
	m_floor->Initialize();
	m_player->SetStageLoad(m_stageLoad.get());
}

void ObjectManager::Update()
{
	//タイルクラスにプレイヤーの位置を送る
	m_normalTile->SetPosition(m_position);
	//タイルクラスの更新
	m_normalTile->Update();
	//床クラスにプレイヤーの位置を送る
	m_floor->SetPosition(m_position);
	//タイルクラスからタイルに乗ったかどうかを教える
	m_floor->SetFootHoldFlag(m_normalTile->GetFootHoldFlag());
	m_player->SetFootholdFlag(m_normalTile->GetFootHoldFlag());
	//タイルがリセットされたかどうか
	m_player->SetResetPos(m_normalTile->GetTileReset());
}

void ObjectManager::Draw(const DirectX::SimpleMath::Matrix& view,const DirectX::SimpleMath::Matrix& projection)
{
	m_normalTile->Draw(view,projection);
	m_floor->Draw(view, projection);
}