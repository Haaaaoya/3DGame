#pragma once
#include <Keyboard.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include "StepTimer.h"

class EndDeclaration
{
public:
	EndDeclaration();
	~EndDeclaration();
	void Initialize();
	void Update(const DX::StepTimer& timer);
	void MissDraw();
	void Reset();
private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	// Missテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missTexture;
	//キーボード
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;
	//手前のMiss画像の位置
	DirectX::SimpleMath::Vector2 m_frontTexturePosition;
	//奥のMiss画像の位置
	DirectX::SimpleMath::Vector2 m_aboveTexturePosition;

	//Miss画像の縦サイズを変更する変数
	float m_sizeCount;
	//Miss画像が分岐し始めるまでのカウント
	float m_missCount;
	//テクスチャーの透明度
	float m_transparency;
};