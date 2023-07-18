#pragma once
#include<Model.h>
#include<GeometricPrimitive.h>
#include "Game/AABBFor3D.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include "Game/Stage/StageLoad.h"
#include "Game/GameMain.h"
#include "Game/Tile/ObjectBase.h"
#include "Game/PlaySceneCamera.h"
#include "Game/Tile/ObjectManager.h"

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

class StageBase
{
public:
	enum class STAGE : int
	{
		NONE,

		STAGE1,
		STAGE2,
		STAGE3,
		STAGE4,
		STAGE5
	};
public:
	//getter,setter関数
	//========================================================================
	void SetRot(const DirectX::SimpleMath::Matrix& rot) { m_rotation = rot; }

	void SetCameraPos(const DirectX::SimpleMath::Vector3& CamPos) { m_CamPos = CamPos; }

	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }

	void SetPlaySceneCamera(PlaySceneCamera* playSceneCamera) { m_playSceneCamera = playSceneCamera; }

	void SetObjectManager(ObjectManager* objectManager) { m_objectManager = objectManager; }
	//ObjectBase* GetObjectBase() { return m_objectBase.get(); }

	//タイルをリセットする変数を送る関数
	bool GetResetPos() { return m_tileReset; }
	//プレイヤーがタイルに乗ったかどうか判定する変数を送る関数
	bool GetFootholdFlag() { return m_footholdFlag; }
	//プレイヤーが落ちたかどうか判定する変数を送る関数
	bool GetDropFlag() { return m_dropFlag; }
	//このステージが終わったかどうか判定する変数を送る関数
	bool GetFinish() { return m_finish; }
	//全てのタイルをリセットしたかどうかをゲットする為の関数
	bool GetAllResetFlag() { return m_allResetFlag; }
	//ステージを初期化するかどうかの変数をセットする為の関数
	void SetStartTile(const bool& startTileFlag) { m_startTileFlag = startTileFlag; }
	//落ちたかどうかをゲットする為の関数
	bool GetFallFlag() { return m_fallFlag; }

	//足場から離れているかどうかのフラグをゲットする関数
	bool GetStartFootHold() { return m_startFootHold; }

	float GetRemoveCount() { return m_removeCount; }

	void SetStageChange(const bool& stageChange) { m_stageChange = stageChange; }
	//========================================================================

	StageBase();
	virtual ~StageBase();

	virtual void Initialize();

	virtual STAGE Update(const DX::StepTimer& timer);
	// 描画
	virtual void Draw();

	// 終了処理
	virtual void Finalize();

	void TileReset();

	float FinishTimer(){ return m_finishTimer; }
	
	//エフェクトの初期設定
	void CreateEffect();

protected:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	AABBFor3D* m_pAABBObjectA;
	AABBFor3D* m_pAABBObjectB;

	// ジオメトリプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_pGeometricPrimitive;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	//踏む前の状態のタイルのモデル読み込み用変数
	std::unique_ptr<DirectX::Model> m_pTile;
	//踏んだ後の状態のタイルのモデル読み込み用変数
	std::unique_ptr<DirectX::Model> m_pAfterTile;

	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	std::unique_ptr<StageLoad> m_stageLoad;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blackTexture;

	//カメラの位置情報など
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Vector3 m_target;
	DirectX::SimpleMath::Vector3 m_up;

	std::vector<int> m_tileCount;

	std::vector<bool> m_tileFlag;
	bool m_tileReset;

	//タイルを踏む前に足場から落ちたかどうかで使うフラグ
	//true→足場に乗っている、false→足場から落ちた判定とする
	//タイルを踏み始めたらこのフラグは使わない
	bool m_startFootHold;

	//タイルを踏み始めたかどうかを判断する
	bool m_footholdFlag;
	//プレイヤーが落ちたかどうかを判断する
	bool m_dropFlag;

	bool m_finish;

	//ステージが切り替わる際、タイルを既に踏んでいるというバグを
	//直すためだけに必要なフラグ
	bool m_startTileFlag;

	//フェードアウトが完了するまでゲームを終わらせない為のタイマー
	float m_finishTimer;

	//カメラの位置を切り替えるフラグ
	bool m_cameraMode;

	//タイルを全てリセットするフラグ
	bool m_allResetFlag;

	//再び動き出せるまでの時間
	float m_removeCount;
	//プレイヤーのポジションを受け取るための変数
	DirectX::SimpleMath::Vector3 m_position;

	DirectX::SimpleMath::Vector3 m_InitialCamPos;
	DirectX::SimpleMath::Vector3 m_CamPos;
	DirectX::SimpleMath::Matrix m_rotation;

	//Effekseerで使用するパーツをメンバ変数に追加する
	//レンダラ
	EffekseerRendererDX11::RendererRef m_renderer;
	//マネージャ
	Effekseer::ManagerRef m_manager;
	//エフェクト
	Effekseer::EffectRef m_effect;
	//ハンドル
	Effekseer::Handle m_handle;

	//エフェクトを表示するときに使う
	std::vector<int> m_effectCount;

	//タイルをリセットするまでのカウント
	int m_allResetCount;

	//落ちたかどうか判定するフラグ
	bool m_fallFlag;

	int m_tileResetCount;

	//ステージ切り替えのフラグを受け取る変数
	bool m_stageChange;

	//ステージデータを格納する変数
	int m_stageData[7][4];

	//CSVの行数を受け取る変数
	DirectX::SimpleMath::Vector2 m_stagePos;

	std::unique_ptr<ObjectBase> m_objectBase;

	PlaySceneCamera* m_playSceneCamera;

	ObjectManager* m_objectManager;
};