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
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	//	�X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeybord;
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;
};