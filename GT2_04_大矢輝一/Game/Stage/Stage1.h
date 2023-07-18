#pragma once

#include<GeometricPrimitive.h>
#include "Game/AABBFor3D.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>

#include "Game/GameMain.h"

#include <CommonStates.h>
#include<Model.h>

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "Game/Stage/StageBase.h"
#include "Game/PlaySceneCamera.h"

class Stage1:public StageBase
{
private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;

	AABBFor3D* m_pAABBObjectA;
	std::unique_ptr<AABBFor3D> m_pAABBObjectB;

	// ジオメトリプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive> m_pGeometricPrimitive;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;
public:
	// コンストラクタ
	Stage1();

	// デストラクタ
	~Stage1() override;

	// 初期化
	void Initialize() override;

	// 更新
	STAGE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	bool AABB(DirectX::SimpleMath::Vector3 a1, DirectX::SimpleMath::Vector3 a2,
		DirectX::SimpleMath::Vector3 b1, DirectX::SimpleMath::Vector3 b2);

	////タイルをリセットする変数を送る関数
	//bool GetResetPos();
	////プレイヤーがタイルに乗ったかどうか判定する変数を送る関数
	//bool GetFootholdFlag();
	////プレイヤーが落ちたかどうか判定する変数を送る関数
	//bool GetDropFlag();
	////このステージが終わったかどうか判定する変数を送る関数
	//bool GetFinish();
	////全てのタイルをリセットしたかどうかをゲットする為の関数
	//bool GetAllResetFlag() { return m_allResetFlag; }
	////落ちたかどうかをゲットする為の関数
	//bool GetFallFlag() { return m_fallFlag; }

	////足場から離れているかどうかのフラグをゲットする関数
	//bool GetStartFootHold();

	//float GetRemoveCount();
	////ゲーム終了のタイマー情報の関数
	//float FinishTimer();
};