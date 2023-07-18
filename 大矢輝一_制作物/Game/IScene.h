/*
シーンインターフェース
*/
#pragma once

#include "StepTimer.h"
#include "GameMain.h"


class IScene
{
protected:
	bool m_clear;
	int m_stageNumber;
public:

	virtual ~IScene() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual GAME_SCENE Update(const DX::StepTimer& timer) = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	virtual bool GetClearFlag() = 0;

	virtual int GetStageNum() = 0;

	// リソースの読み込み
	//画像の読み込みはここで
	virtual void LoadResources() = 0;
};