#pragma once

#include "DeviceResources.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include "Game/IScene.h"
#include "Game/Stage/StageLoad.h"
#include "Game/ResourcesLoad.h"

class StageCreate : public IScene
{
public:

	StageCreate();
	~StageCreate();

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
	//CSVデータを0で初期化
	void StageDataReset();

	DirectX::SimpleMath::Vector2 Clamp(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& min, 
		const DirectX::SimpleMath::Vector2& max);
private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	//	スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//キーボード
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	//ステージロードクラス
	std::unique_ptr<StageLoad> m_stageLoad;
	//フレーム画像のポジション
	DirectX::SimpleMath::Vector2 m_framePosition;
	//選択フラグ
	bool m_choiceFlag;
	//保存画面に移動するかどうか
	bool m_saveFlag;
	//テクスチャーのポジション
	DirectX::SimpleMath::Vector2 m_showPosition;

	int m_stageData[8][8];

	//アルファ値と切り替えるためのフラグ
	bool m_alphaFlag;
	//アルファ値変数
	float m_alpha;

	//「はい」のポジション
	DirectX::SimpleMath::Vector2 m_yesPosition;
	//「いいえ」のポジション
	DirectX::SimpleMath::Vector2 m_noPosition;
	//フレーム画像の動く時間
	float m_moveTimer;

	//CSVデータを書き換えるための変数を用意
	std::vector<std::vector<int>> m_csvData;
};