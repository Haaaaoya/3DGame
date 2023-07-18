#include "pch.h"
#include <WICTextureLoader.h>
#include "EndDeclaration.h"
#include "DeviceResources.h"

EndDeclaration::EndDeclaration():
	m_sizeCount(0.0f),
	m_frontTexturePosition{0,0},
	m_aboveTexturePosition{0,0},
	m_missCount(0.0f),
	m_transparency(1.0f)
{
}

EndDeclaration::~EndDeclaration()
{
}

void EndDeclaration::Initialize()
{
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
		L"Resources/Textures/Miss.png",
		nullptr,
		m_missTexture.ReleaseAndGetAddressOf()
	);

	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
}

void EndDeclaration::Update(const DX::StepTimer& timer)
{
	float time = static_cast<float>(timer.GetElapsedSeconds());
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);

	static const float maxSize = 1.0f;
	static const float maxCount = 1.0f;
	
	m_sizeCount += 1 / 20.0f;

	//Miss画像が描画される
	if (m_sizeCount >= maxSize)
	{
		m_sizeCount = maxSize;
		m_missCount += time;
	}
	if (m_missCount >= maxCount)
	{
		m_frontTexturePosition.x++;
		m_aboveTexturePosition.x--;
		m_transparency -= time;
	}
	//「Miss」画像の位置と透明度をリセットする
	if (m_missCount >= 4.7f)
	{
		Reset();
	}
}

void EndDeclaration::MissDraw()
{
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied(),
		nullptr, nullptr, nullptr, nullptr,
		DirectX::SimpleMath::Matrix::CreateScale(1, m_sizeCount, 1) * DirectX::SimpleMath::Matrix::CreateTranslation(700, 400, 0));
	//Miss画像描画
	m_spriteBatch->Draw(m_missTexture.Get(), m_frontTexturePosition,
		0, DirectX::SimpleMath::Vector4(1, 1, 1, m_transparency), 0.0f, DirectX::SimpleMath::Vector2(100, 50), 2.0f,DirectX::SpriteEffects_None, 1.0f);
	//Miss画像描画
	m_spriteBatch->Draw(m_missTexture.Get(), m_aboveTexturePosition,
		0, DirectX::SimpleMath::Vector4(1, 1, 1, m_transparency), 0.0f, DirectX::SimpleMath::Vector2(100, 50), 2.0f, DirectX::SpriteEffects_None, 1.0f);
	m_spriteBatch->End();
}

void EndDeclaration::Reset()
{
	m_sizeCount = 0.0f;
	m_missCount = 0.0f;
	m_frontTexturePosition = { 0,0 };
	m_aboveTexturePosition = { 0,0 };
	m_transparency = 1.0f;
}