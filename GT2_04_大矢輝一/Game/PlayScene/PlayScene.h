/*
プレイシーン
*/
#pragma once

#include <CommonStates.h>

#include "../IScene.h"
#include "Game/GameMain.h"


#include<Model.h>
#include<GeometricPrimitive.h>
#include "Game/AABBFor3D.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include "Player.h"
#include <Game/ShaderSample.h>
#include "Game/EffectManager.h"
#include <Game/P_ShaderSample.h>
#include "Game/P_EffectManager.h"
#include "Game/LoadingScreen/LoadingScreen.h"
#include "Game/Stage/Stage1.h"
#include "Game/Stage/StageBase.h"
#include "Game/EndDeclaration/EndDeclaration.h"
#include <DeviceResources.h>
#include "Game/Tile/ObjectManager.h"
#include "Game/PlaySceneCamera.h"
#include "Game/ResourcesLoad.h"

class DebugCamera;

enum Stage
{
	NONE,

	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE
};

class PlayScene : public IScene
{
public:
	//getter,setter関数
	//========================================================================
	//ゲームクリアしたかどうか
	bool GetClearFlag() override;

	//何ステージ目を選択してるか
	int GetStageNum() override;

	// リソースの読み込み
	void LoadResources() override;

	void SetState();

	void SetResetPos(const bool& reset) { m_tileReset = reset; }
	void Fade();

	void CreateStage();

	void DeleteStage();
	//========================================================================

	// コンストラクタ
	PlayScene(int stageNumber);

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	// カメラ
	std::unique_ptr<DebugCamera>					m_debugCamera;

	std::unique_ptr<DirectX::Model>m_pModel;

	std::unique_ptr<DirectX::Model>m_pBack;

	DirectX::SimpleMath::Vector3 m_position;

	//	スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	AABBFor3D* m_pAABBObjectA;
	AABBFor3D* m_pAABBObjectB;

	std::unique_ptr<Player> m_player;
	std::unique_ptr<StageBase> m_pStageBase;
	StageBase::STAGE m_nextStage;
	//終了宣言クラスのポインタ
	std::unique_ptr<EndDeclaration> m_endDeclaration;

	//タイル、床のマネージャ
	std::unique_ptr<ObjectManager> m_objectManager;

	//プレイシーンカメラ
	std::unique_ptr<PlaySceneCamera> m_playSceneCamera;

	//クリアしたかどうか
	bool m_clear;
	//タイルを踏む前に足場から落ちたかどうかで使うフラグ
	//true→足場に乗っている、false→足場から落ちた判定とする
	//タイルを踏み始めたらこのフラグは使わない
	bool m_startFootHold;

	//タイルを踏み始めたかどうかを判断する
	bool m_footholdFlag;

	//プレイヤーが落ちたかどうかを判断する
	bool m_dropFlag;


	bool tileFlag[20];

	//タイルをリセットする
	bool m_tileReset;

	bool m_finish;

	//フェードアウトが完了するまでゲームを終わらせない為のタイマー
	float m_finishTimer;


	//ライフの数
	int m_lifeCount;
	bool m_lifeFlag;

	//画像の切り抜き変数
	RECT m_rect;

	bool m_textureFlag;
	float m_textureTimer;
	//ステージが切り替わる際、タイルを既に踏んでいるというバグを
	//直すためだけに必要なフラグ
	bool m_startTileFlag;

	//再び動き出せるまでの時間
	float m_removeCount;

	//カメラの位置を切り替えるフラグ
	bool m_cameraMode;

	//説明書の表示を切り替えるフラグ
	//3段階に分けたいためint型
	int m_manualFlag;

	std::unique_ptr<EffectManager> m_effectMane;

	// デバイスリソース
	DX::DeviceResources* m_deviceResources;

	// キーボード
	DirectX::Keyboard* m_pKeybord;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keybordState;
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	// ジオメトリプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_pGeometricPrimitive;

	int m_angle = 0;

	DirectX::SimpleMath::Vector3 m_InitialCamPos;

	DirectX::SimpleMath::Vector3 m_CamPos;

	DirectX::SimpleMath::Vector3 m_vel;

	DirectX::SimpleMath::Matrix m_rotation;

	//エフェクトを表示するときに使う
	int m_effectCount;

	//カメラを動かすフラグ
	//左回転
	bool m_cameraLeftFlag;
	//右回転
	bool m_cameraRightFlag;
	//左回転するカウント
	int m_cameraLeftCount;
	//右回転するカウント
	int m_cameraRightCount;
	//ステージを切り替える変数
	bool m_stageChange;
};