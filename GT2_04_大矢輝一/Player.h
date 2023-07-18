#pragma once

#include <CommonStates.h>
#include "Libraries/MyLibraries/DebugCamera.h"
#include "Game/IScene.h"

#include <SimpleMath.h>
#include <Game/P_ShaderSample.h>
#include<Model.h>
#include "Game/P_EffectManager.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include "Game/PlaySceneCamera.h"
#include "Game/Stage/StageLoad.h"
#include "Animation/Animation.h"
class Player
{
public:
	enum direction
	{
		RIGHT,
		LEFT,
		UP,
		DOWN
	};

	//getter,setter関数
	//========================================================================
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	void SetResetPos(const bool& reset) { m_resetPosition = reset; }

	void SetFootholdFlag(const bool& footholdFlag) { m_footholdFlag = footholdFlag; }

	void SetDropFlag(const bool& dropFlag) { m_dropFlag = dropFlag; }

	void SetTextureFlag(const bool& textureFlag) { m_textureFlag = textureFlag; }

	void SetStartFootHold(const bool& startfootHold) { m_startFootHold = startfootHold; }

	bool GetInputStopFlag() { return m_inputStopFlag; }
	//プレイヤーが動くために必要なカウントを
	//リセットする変数を受け取る
	void SetVelCountReset(const bool& velCountReset) { m_velCountReset = velCountReset; }

	//お落ちたかどうか受け取る関数
	void SetFallFlag(const bool& fallFlag) { m_fallFlag = fallFlag; }
	//========================================================================

	void SetPlaySceneCamera(PlaySceneCamera* playSceneCamera) { m_playSceneCamera = playSceneCamera; }
	void SetStageLoad(StageLoad* stageLoad) { m_stageLoad = stageLoad; }

	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化
	void Initialize();

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	/*void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);*/
	void Draw();
	// 終了処理
	void Finalize();
	//キャラが動けるかどうか
	bool PlayerMove();
	//keyFlagをクリアする関数
	void ClearKeyFlag();
private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;
	//プレイヤーのポジション
	DirectX::SimpleMath::Vector3 m_position;
	//初期のプレイヤーのポジション
	DirectX::SimpleMath::Vector3 m_initialPosition;
	DebugCamera* m_pDebugCamera;
	//プレイヤーモデル4種類
	std::unique_ptr<DirectX::Model>m_pModel;
	std::unique_ptr<DirectX::Model>m_pModelLeft;
	std::unique_ptr<DirectX::Model>m_pModelRight;
	std::unique_ptr<DirectX::Model>m_pModelFall;

	int m_modelTimer;

	// キーボード
	DirectX::Keyboard* m_pKeybord;
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	//	スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//カメラのアングル
	int m_angle;

	std::unique_ptr<PlayerEffectManager> m_effectManager;

	//プレイシーンカメラ
	PlaySceneCamera* m_playSceneCamera;
	//ステージロードクラス
	StageLoad* m_stageLoad;
	//プレイヤーの回転情報を保存するための変数(world保存用変数)
	DirectX::SimpleMath::Matrix m_proveWorld;
	//プレーヤーの初期位置
	bool m_resetPosition;
	//足場が消えたかどうかを判定するフラグ
	bool m_footholdFlag;
	//落ちたかどうか判定するフラグ
	bool m_dropFlag;

	//フェード中にプレイヤーが操作できないようにするために必要
	bool m_textureFlag;

	//プレイヤーが足場から落ちたかどうかのフラグ
	bool m_startFootHold;

	//プレイヤーが落ちたとみなされたかどうかの判定
	bool m_fallFlag;
	//プレイヤー用のアングル変数
	float m_playerAngle;

	//プレイヤーの4方向ベロシティ
	int m_velocityCount;
	//ベロシティが有効なフラグ
	bool m_upFlag;
	bool m_underFlag;
	bool m_leftFlag;
	bool m_rightFlag;

	//キャラが動いてる最中、他の力を加えさせないフラグ
	bool m_inputStopFlag;

	//カメラを動かすフラグ
	//左回転
	bool m_cameraLeftFlag;
	//右回転
	bool m_cameraRightFlag;
	//左回転するカウント
	int m_cameraLeftCount;
	//右回転するカウント
	int m_cameraRightCount;
	//プレイヤーが動くために必要なカウントをリセットする変数
	bool m_velCountReset;

	//「Miss」画像表示中のカウント
	int m_endDecCount;

	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_fxFactory;

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// SDKアニメーション
	std::unique_ptr<DX::AnimationSDKMESH> m_animation;

	// 変換配列
	DirectX::ModelBone::TransformArray m_transformArray;

	bool m_keyFlag[4];
};