/*
ロード画面
*/

#include "pch.h"
#include <WICTextureLoader.h>
#include "LoadingScreen.h"
#include <SimpleMath.h>


#include "DeviceResources.h"
#include "../IScene.h"
#include <SimpleMath.h>

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
LoadingScreen::LoadingScreen()
	:
	m_isLoading{false},
	count(0.0f),
	m_charaRECT{0,224,32,256},
	m_moveCount(0.0f),
	m_textureCount(0.0f),
	posY(3,-100),
	m_rightBigMove(610.0f),
	m_rightShortMove(1150.0f),
	m_leftBigMove(90.0f),
	m_leftShortMove(-30.0f)
{
	for (int i = 0; i < 3; i++)
	{
		posY[i] = posY[i] * (i * 2);
	}
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
LoadingScreen::~LoadingScreen()
{
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void LoadingScreen::Initialize(IScene* targetScene)
{
	// 対象シーンのリソースの読み込み
	LoadSceneResources(targetScene);


	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");


	// テクスチャの読み込み
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/NowLoading.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/rightBigDoor.png",
		nullptr,
		m_rightBigDoor.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/rightShortDoor.png",
		nullptr,
		m_rightShortDoor.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/leftBigDoor.png",
		nullptr,
		m_leftBigDoor.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/leftShortDoor.png",
		nullptr,
		m_leftShortDoor.ReleaseAndGetAddressOf()
	);

	m_rightBigPosition = DirectX::SimpleMath::Vector2(m_rightBigMove, -100.0f);
	m_rightShortPosition = DirectX::SimpleMath::Vector2(m_rightShortMove, -100.0f);
	m_leftBigPosition = DirectX::SimpleMath::Vector2(m_leftBigMove, -100.0f);
	m_leftShortPosition = DirectX::SimpleMath::Vector2(m_leftShortMove, -100.0f);
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
void LoadingScreen::Update(const DX::StepTimer& timer)
{
	
	


	//キャラの画像を切り抜き
	if (m_moveCount >= 0.0f && m_moveCount < 40.0f)
	{
		m_charaRECT = { 64,224,96,256 };
	}
	if (m_moveCount >= 40.0f && m_moveCount < 80.0f)
	{
		m_charaRECT = { 32,224,64,256 };
	}
	if (m_moveCount >= 80.0f && m_moveCount < 120.0f)
	{
		m_charaRECT = { 0,224,32,256 };
	}

	if (m_moveCount > 120.0f)
	{
		m_moveCount = 0.0f;
	}
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void LoadingScreen::Draw()
{
	
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	
	m_spriteBatch->Draw(m_rightBigDoor.Get(), m_rightBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(m_rightShortDoor.Get(), m_rightShortPosition, 0, DirectX::Colors::White);

	m_spriteBatch->Draw(m_leftBigDoor.Get(), m_leftBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(m_leftShortDoor.Get(), m_leftShortPosition, 0, DirectX::Colors::White);

	m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(850, 650),
		0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(300, 35), 1.0f);

	m_spriteBatch->End();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void LoadingScreen::Finalize()
{
}

/*--------------------------------------------------
	// 読み込み中かどうかの取得
--------------------------------------------------*/
bool LoadingScreen::IsLoading()
{
	m_LoadingMutex.lock();
	bool isLoading = m_isLoading;
	m_LoadingMutex.unlock();

	return isLoading;
}

/*--------------------------------------------------
シーンのリソースの読み込み
--------------------------------------------------*/
void LoadingScreen::LoadSceneResources(IScene* targetScene)
{
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}

	m_isLoading = true;
	m_LoadingThread = std::thread(
		[=]
		{
			targetScene->LoadResources();

			m_LoadingMutex.lock();
			m_isLoading = false;
			m_LoadingMutex.unlock();
		});
}
