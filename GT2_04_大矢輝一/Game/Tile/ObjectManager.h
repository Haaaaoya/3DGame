#pragma once
#include "Floor.h"
#include "NormalTile.h"
#include "Floor.h"
#include "Game/Stage/StageLoad.h"
#include "Game/PlaySceneCamera.h"
#include "Player.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Initialize(int stageNumber);
	void Update();
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);

	void SetPlaySceneCamera(PlaySceneCamera* playSceneCamera) { m_playSceneCamera = playSceneCamera; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	void SetPlayer(Player* player) { m_player = player; }
	//全ての処理をリセットする関数
	bool GetAllResetFlag() { return m_normalTile->GetAllResetFlag(); }

	float GetRemoveCount() { return m_normalTile->GetRemoveCount(); }

	//クリア判定をゲットする関数
	bool GetClearFlag() { return m_normalTile->GetClearFlag(); }

	StageLoad* GetStageLoad() { return m_stageLoad.get(); }
private:
	std::unique_ptr<StageLoad> m_stageLoad;
	std::unique_ptr<NormalTile> m_normalTile;
	std::unique_ptr<Floor> m_floor;
	Player* m_player;
	PlaySceneCamera* m_playSceneCamera;

	//プレイヤーのポジションを受け取る変数
	DirectX::SimpleMath::Vector3 m_position;
};