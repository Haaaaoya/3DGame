/*
���U���g�V�[��
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
	// �R���X�g���N�^
	ResultScene(bool clear);

	// �f�X�g���N�^
	~ResultScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;
	//�Q�[���N���A�������ǂ���
	bool GetClearFlag() override;
	//���X�e�[�W�ڂ�I�����Ă邩
	int GetStageNum() override;

	// ���\�[�X�̓ǂݍ���
	void LoadResources() override;
private:

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	bool m_clear;

	bool m_gameOverFlag;

	RECT m_rect[5];
	//�uClear�v�̃e�N�X�`���[���o�鎞��
	int m_textureCount;
	//�摜�̊g�嗦
	std::vector<float> m_scale{ 10.0f,10.0f,10.0f,10.0f,10.0f };
	//�����x
	float m_alpha;
	//���̔z��
	bool m_seFlag[5];
	//�X�P�[���̌Œ�l
	float m_scaleValue = 0.6f;
	//���摜�̈ʒu�Ɋւ��Ă̕ϐ�
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