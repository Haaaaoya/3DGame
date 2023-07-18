/*
リザルトシーン
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include "Game/PlayScene/PlayScene.h"

class ResultScene :public IScene
{
public:
	// コンストラクタ
	ResultScene(bool clear);

	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;
	//ゲームクリアしたかどうか
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

	bool m_clear;

	bool m_gameOverFlag;

	RECT m_rect[5];
	//「Clear」のテクスチャーが出る時間
	int m_textureCount;
	//画像の拡大率
	std::vector<float> m_scale{ 10.0f,10.0f,10.0f,10.0f,10.0f };
	//透明度
	float m_alpha;
	//音の配列
	bool m_seFlag[5];
	//スケールの固定値
	float m_scaleValue = 0.6f;
	//扉画像の位置に関しての変数
	//====================================================
	DirectX::SimpleMath::Vector2 m_rightBigMove;
	DirectX::SimpleMath::Vector2 m_rightShortMove;
	DirectX::SimpleMath::Vector2 m_leftBigMove;
	DirectX::SimpleMath::Vector2 m_leftShortMove;

	DirectX::SimpleMath::Vector2 m_rightBigPosition;
	DirectX::SimpleMath::Vector2 m_rightShortPosition;
	DirectX::SimpleMath::Vector2 m_leftBigPosition;
	DirectX::SimpleMath::Vector2 m_leftShortPosition;
	//====================================================
};