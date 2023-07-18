/*
ロード画面
*/
#pragma once

#include <thread>
#include <mutex>

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include <SimpleMath.h>

#include "../ILoadingScreen.h"
#include <vector>


class LoadingScreen : public ILoadingScreen
{
private:

	// リソース読み込み用
	std::thread m_LoadingThread;
	bool m_isLoading;
	std::mutex m_LoadingMutex;

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// 右の大きな扉テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rightBigDoor;

	// 右の小さな扉テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rightShortDoor;

	// 左の大きな扉テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_leftBigDoor;

	// 左の小さな扉テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_leftShortDoor;

	
	float count;
	//キャラを動かすカウント
	float m_moveCount;
	//背景の切り替えカウント
	float m_textureCount;

	DirectX::SimpleMath::Vector2 m_rightBigPosition;
	DirectX::SimpleMath::Vector2 m_rightShortPosition;
	DirectX::SimpleMath::Vector2 m_leftBigPosition;
	DirectX::SimpleMath::Vector2 m_leftShortPosition;

	//それぞれの扉の位置の固定値
	float m_rightBigMove;
	float m_rightShortMove;
	float m_leftBigMove;
	float m_leftShortMove;

	//雪のY座標
	std::vector<float> posY;
	

	RECT m_charaRECT;

public:

	// コンストラクタ
	LoadingScreen();

	// デストラクタ
	~LoadingScreen();

	// 初期化
	void Initialize(IScene* targetScene) override;

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 読み込み中かどうかの取得
	bool IsLoading();
private:
	// 対象シーンのリソースの読み込み
	void LoadSceneResources(IScene* targetScene) override;

};