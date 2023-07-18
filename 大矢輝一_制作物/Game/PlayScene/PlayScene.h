/*
�v���C�V�[��
*/
#pragma once

#include <CommonStates.h>

#include "../IScene.h"
#include "Game/GameMain.h"


#include<Model.h>
#include<GeometricPrimitive.h>
#include "Game/AABBFor3D.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include "Player.h"
#include <Game/ShaderSample.h>
#include "Game/EffectManager.h"
#include <Game/P_ShaderSample.h>
#include "Game/P_EffectManager.h"
#include "Game/LoadingScreen/LoadingScreen.h"
#include "Game/Stage/Stage1.h"
#include "Game/Stage/StageBase.h"
#include "Game/EndDeclaration/EndDeclaration.h"
#include <DeviceResources.h>
#include "Game/Tile/ObjectManager.h"
#include "Game/PlaySceneCamera.h"
#include "Game/ResourcesLoad.h"

class DebugCamera;

enum Stage
{
	NONE,

	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE
};

class PlayScene : public IScene
{
public:
	//getter,setter�֐�
	//========================================================================
	//�Q�[���N���A�������ǂ���
	bool GetClearFlag() override;

	//���X�e�[�W�ڂ�I�����Ă邩
	int GetStageNum() override;

	// ���\�[�X�̓ǂݍ���
	void LoadResources() override;

	void SetState();

	void SetResetPos(const bool& reset) { m_tileReset = reset; }
	void Fade();

	void CreateStage();

	void DeleteStage();
	//========================================================================

	// �R���X�g���N�^
	PlayScene(int stageNumber);

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	// �J����
	std::unique_ptr<DebugCamera>					m_debugCamera;

	std::unique_ptr<DirectX::Model>m_pModel;

	std::unique_ptr<DirectX::Model>m_pBack;

	DirectX::SimpleMath::Vector3 m_position;

	//	�X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	AABBFor3D* m_pAABBObjectA;
	AABBFor3D* m_pAABBObjectB;

	std::unique_ptr<Player> m_player;
	std::unique_ptr<StageBase> m_pStageBase;
	StageBase::STAGE m_nextStage;
	//�I���錾�N���X�̃|�C���^
	std::unique_ptr<EndDeclaration> m_endDeclaration;

	//�^�C���A���̃}�l�[�W��
	std::unique_ptr<ObjectManager> m_objectManager;

	//�v���C�V�[���J����
	std::unique_ptr<PlaySceneCamera> m_playSceneCamera;

	//�N���A�������ǂ���
	bool m_clear;
	//�^�C���𓥂ޑO�ɑ��ꂩ�痎�������ǂ����Ŏg���t���O
	//true������ɏ���Ă���Afalse�����ꂩ�痎��������Ƃ���
	//�^�C���𓥂ݎn�߂��炱�̃t���O�͎g��Ȃ�
	bool m_startFootHold;

	//�^�C���𓥂ݎn�߂����ǂ����𔻒f����
	bool m_footholdFlag;

	//�v���C���[�����������ǂ����𔻒f����
	bool m_dropFlag;


	bool tileFlag[20];

	//�^�C�������Z�b�g����
	bool m_tileReset;

	bool m_finish;

	//�t�F�[�h�A�E�g����������܂ŃQ�[�����I��点�Ȃ��ׂ̃^�C�}�[
	float m_finishTimer;


	//���C�t�̐�
	int m_lifeCount;
	bool m_lifeFlag;

	//�摜�̐؂蔲���ϐ�
	RECT m_rect;

	bool m_textureFlag;
	float m_textureTimer;
	//�X�e�[�W���؂�ւ��ہA�^�C�������ɓ���ł���Ƃ����o�O��
	//�������߂����ɕK�v�ȃt���O
	bool m_startTileFlag;

	//�Ăѓ����o����܂ł̎���
	float m_removeCount;

	//�J�����̈ʒu��؂�ւ���t���O
	bool m_cameraMode;

	//�������̕\����؂�ւ���t���O
	//3�i�K�ɕ�����������int�^
	int m_manualFlag;

	std::unique_ptr<EffectManager> m_effectMane;

	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;

	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeybord;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keybordState;
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	// �W�I���g���v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive> m_pGeometricPrimitive;

	int m_angle = 0;

	DirectX::SimpleMath::Vector3 m_InitialCamPos;

	DirectX::SimpleMath::Vector3 m_CamPos;

	DirectX::SimpleMath::Vector3 m_vel;

	DirectX::SimpleMath::Matrix m_rotation;

	//�G�t�F�N�g��\������Ƃ��Ɏg��
	int m_effectCount;

	//�J�����𓮂����t���O
	//����]
	bool m_cameraLeftFlag;
	//�E��]
	bool m_cameraRightFlag;
	//����]����J�E���g
	int m_cameraLeftCount;
	//�E��]����J�E���g
	int m_cameraRightCount;
	//�X�e�[�W��؂�ւ���ϐ�
	bool m_stageChange;
};