#pragma once

#include <CommonStates.h>
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <Keyboard.h>

class Manual
{
public:
	Manual();
	~Manual();
	void Initialize();
	void Update();
	void Draw();
private:
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	//	スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// キーボード
	DirectX::Keyboard* m_pKeybord;
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;
};