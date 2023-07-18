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
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	// Miss�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_missTexture;
	//�L�[�{�[�h
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;
	//��O��Miss�摜�̈ʒu
	DirectX::SimpleMath::Vector2 m_frontTexturePosition;
	//����Miss�摜�̈ʒu
	DirectX::SimpleMath::Vector2 m_aboveTexturePosition;

	//Miss�摜�̏c�T�C�Y��ύX����ϐ�
	float m_sizeCount;
	//Miss�摜�����򂵎n�߂�܂ł̃J�E���g
	float m_missCount;
	//�e�N�X�`���[�̓����x
	float m_transparency;
};