#pragma once

#include "DeviceResources.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include "Game/IScene.h"
#include "Game/Stage/StageLoad.h"
#include "Game/ResourcesLoad.h"

class StageCreate : public IScene
{
public:

	StageCreate();
	~StageCreate();

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
	//CSV�f�[�^��0�ŏ�����
	void StageDataReset();

	DirectX::SimpleMath::Vector2 Clamp(const DirectX::SimpleMath::Vector2& a, const DirectX::SimpleMath::Vector2& min, 
		const DirectX::SimpleMath::Vector2& max);
private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	//	�X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//�L�[�{�[�h
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	//�X�e�[�W���[�h�N���X
	std::unique_ptr<StageLoad> m_stageLoad;
	//�t���[���摜�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_framePosition;
	//�I���t���O
	bool m_choiceFlag;
	//�ۑ���ʂɈړ����邩�ǂ���
	bool m_saveFlag;
	//�e�N�X�`���[�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_showPosition;

	int m_stageData[8][8];

	//�A���t�@�l�Ɛ؂�ւ��邽�߂̃t���O
	bool m_alphaFlag;
	//�A���t�@�l�ϐ�
	float m_alpha;

	//�u�͂��v�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_yesPosition;
	//�u�������v�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_noPosition;
	//�t���[���摜�̓�������
	float m_moveTimer;

	//CSV�f�[�^�����������邽�߂̕ϐ���p��
	std::vector<std::vector<int>> m_csvData;
};