#pragma once
#include <Model.h>
#include <SimpleMath.h>
#include <CommonStates.h>
#include "Game/Stage/StageLoad.h"
#include "Game/PlaySceneCamera.h"

class ObjectBase
{
public:
	enum class TILE :int
	{
		NONE,

		NORMAL,
		FLOOR
	};

	ObjectBase();
	~ObjectBase();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	//get関数
	DirectX::SimpleMath::Vector3 GetEye() { return m_eye; }
	DirectX::SimpleMath::Vector3 GetTarget() { return m_target; }
	DirectX::SimpleMath::Vector3 GetUp() { return m_up; }
	// コモンステート
	DirectX::CommonStates* GetCommonState(){ return m_commonState.get(); }
	DirectX::SimpleMath::Matrix GetRot() { return m_rotation; }
	DirectX::SimpleMath::Vector3 GetCameraPosition() { return m_CamPos; }
	DirectX::SimpleMath::Vector3 GetInitialCameraPosition() { return m_InitialCamPos; }
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	PlaySceneCamera* GetPlaySceneCamera() { return m_playSceneCamera; }
	StageLoad* GetStageLoad() { return m_stageLoad; }
	//set関数
	void SetEye(const DirectX::SimpleMath::Vector3& eye) { m_eye = eye; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	void SetStageLoad(StageLoad* stageLoad) { m_stageLoad = stageLoad; }
	void SetPlaySceneCamera(PlaySceneCamera* playSceneCamera) { m_playSceneCamera = playSceneCamera; }
private:
	//踏む前の状態のタイルのモデル読み込み用変数
	std::unique_ptr<DirectX::Model> m_pModel;

	//カメラの位置情報など
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Vector3 m_target;
	DirectX::SimpleMath::Vector3 m_up;

	DirectX::SimpleMath::Vector3 m_InitialCamPos;
	DirectX::SimpleMath::Vector3 m_CamPos;
	DirectX::SimpleMath::Matrix m_rotation;

	//プレイヤーのポジションを受け取るための変数
	DirectX::SimpleMath::Vector3 m_position;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	//プレイシーンカメラ
	PlaySceneCamera* m_playSceneCamera;
	//ステージロード
	StageLoad* m_stageLoad;
};