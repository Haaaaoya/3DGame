/*
�^�C�g���V�[��
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include "../IScene.h"
#include "Game/GameMain.h"
#include<Model.h>
#include "Game/EndDeclaration/EndDeclaration.h"
#include "Game/ResourcesLoad.h"

class TitleScene : public IScene
{
public:

	/*enum Texture
	{
		rightBigDoor,
		rightShortDoor,
		leftBigDoor,
		leftShortDoor,
		ZKey,
		frame,
		GameStart,
		StageCreate,
		Afewline,
		StageSelect,
		Arrow,

		TextureNum
	};*/

	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

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
	//�L�[�{�[�h
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	//GameStart�摜�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_GameStartPosition;
	//GameStart�摜�̓�������
	float m_GameStartTimer;

	//StageCreate�摜�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_StageCreatePosition;
	//StageCreate�摜�̓�������
	float m_StageCreateTimer;

	//���摜�̈ʒu�Ɋւ��Ă̕ϐ�
	//====================================================
	//�x���V�e�B
	DirectX::SimpleMath::Vector2 m_rightBigMove;
	DirectX::SimpleMath::Vector2 m_rightShortMove;
	DirectX::SimpleMath::Vector2 m_leftBigMove;
	DirectX::SimpleMath::Vector2 m_leftShortMove;
	//�|�W�V����
	DirectX::SimpleMath::Vector2 m_rightBigPosition;
	DirectX::SimpleMath::Vector2 m_rightShortPosition;
	DirectX::SimpleMath::Vector2 m_leftBigPosition;
	DirectX::SimpleMath::Vector2 m_leftShortPosition;
	//====================================================
	//�v���C�V�[���ɍs���܂ł̎���
	float m_changeTimer;

	//�Q�[���X�^�[�g�t���O
	bool m_startFlag;
	//�t���[���摜�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_framePosition;
	//�X�e�[�W����邩�ǂ����t���O
	bool m_stageCreateFlag;
	//�X�e�[�W��I�����Ă邩�ǂ����̃t���O
	bool m_stageChoiceFlag;
	//���摜�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_arrowTexturePosition;
	//�A���t�@�l�Ɛ؂�ւ��邽�߂̃t���O
	bool m_alphaFlag;
	//�A���t�@�l�ϐ�
	float m_alpha;
};