/*
タイトルシーン
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include<Model.h>
#include "Game/EndDeclaration/EndDeclaration.h"
#include "Game/ResourcesLoad.h"

class TitleScene : public IScene
{
public:

	/*enum Texture
	{
		rightBigDoor,
		rightShortDoor,
		leftBigDoor,
		leftShortDoor,
		ZKey,
		frame,
		GameStart,
		StageCreate,
		Afewline,
		StageSelect,
		Arrow,

		TextureNum
	};*/

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	bool GetClearFlag() override;

	//何ステージ目を選択してるか
	int GetStageNum() override;

	// リソースの読み込み
	void LoadResources() override;
private:

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//キーボード
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	//GameStart画像のポジション
	DirectX::SimpleMath::Vector2 m_GameStartPosition;
	//GameStart画像の動く時間
	float m_GameStartTimer;

	//StageCreate画像のポジション
	DirectX::SimpleMath::Vector2 m_StageCreatePosition;
	//StageCreate画像の動く時間
	float m_StageCreateTimer;

	//扉画像の位置に関しての変数
	//====================================================
	//ベロシティ
	DirectX::SimpleMath::Vector2 m_rightBigMove;
	DirectX::SimpleMath::Vector2 m_rightShortMove;
	DirectX::SimpleMath::Vector2 m_leftBigMove;
	DirectX::SimpleMath::Vector2 m_leftShortMove;
	//ポジション
	DirectX::SimpleMath::Vector2 m_rightBigPosition;
	DirectX::SimpleMath::Vector2 m_rightShortPosition;
	DirectX::SimpleMath::Vector2 m_leftBigPosition;
	DirectX::SimpleMath::Vector2 m_leftShortPosition;
	//====================================================
	//プレイシーンに行くまでの時間
	float m_changeTimer;

	//ゲームスタートフラグ
	bool m_startFlag;
	//フレーム画像のポジション
	DirectX::SimpleMath::Vector2 m_framePosition;
	//ステージを作るかどうかフラグ
	bool m_stageCreateFlag;
	//ステージを選択してるかどうかのフラグ
	bool m_stageChoiceFlag;
	//矢印画像のポジション
	DirectX::SimpleMath::Vector2 m_arrowTexturePosition;
	//アルファ値と切り替えるためのフラグ
	bool m_alphaFlag;
	//アルファ値変数
	float m_alpha;
};