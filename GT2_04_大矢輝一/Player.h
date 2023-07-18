#pragma once

#include <CommonStates.h>
#include "Libraries/MyLibraries/DebugCamera.h"
#include "Game/IScene.h"

#include <SimpleMath.h>
#include <Game/P_ShaderSample.h>
#include<Model.h>
#include "Game/P_EffectManager.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include "Game/PlaySceneCamera.h"
#include "Game/Stage/StageLoad.h"
#include "Animation/Animation.h"
class Player
{
public:
	enum direction
	{
		RIGHT,
		LEFT,
		UP,
		DOWN
	};

	//getter,setter�֐�
	//========================================================================
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	void SetResetPos(const bool& reset) { m_resetPosition = reset; }

	void SetFootholdFlag(const bool& footholdFlag) { m_footholdFlag = footholdFlag; }

	void SetDropFlag(const bool& dropFlag) { m_dropFlag = dropFlag; }

	void SetTextureFlag(const bool& textureFlag) { m_textureFlag = textureFlag; }

	void SetStartFootHold(const bool& startfootHold) { m_startFootHold = startfootHold; }

	bool GetInputStopFlag() { return m_inputStopFlag; }
	//�v���C���[���������߂ɕK�v�ȃJ�E���g��
	//���Z�b�g����ϐ����󂯎��
	void SetVelCountReset(const bool& velCountReset) { m_velCountReset = velCountReset; }

	//�����������ǂ����󂯎��֐�
	void SetFallFlag(const bool& fallFlag) { m_fallFlag = fallFlag; }
	//========================================================================

	void SetPlaySceneCamera(PlaySceneCamera* playSceneCamera) { m_playSceneCamera = playSceneCamera; }
	void SetStageLoad(StageLoad* stageLoad) { m_stageLoad = stageLoad; }

	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// ������
	void Initialize();

	// �X�V
	void Update(const DX::StepTimer& timer);

	// �`��
	/*void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);*/
	void Draw();
	// �I������
	void Finalize();
	//�L�����������邩�ǂ���
	bool PlayerMove();
	//keyFlag���N���A����֐�
	void ClearKeyFlag();
private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;
	//�v���C���[�̃|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	//�����̃v���C���[�̃|�W�V����
	DirectX::SimpleMath::Vector3 m_initialPosition;
	DebugCamera* m_pDebugCamera;
	//�v���C���[���f��4���
	std::unique_ptr<DirectX::Model>m_pModel;
	std::unique_ptr<DirectX::Model>m_pModelLeft;
	std::unique_ptr<DirectX::Model>m_pModelRight;
	std::unique_ptr<DirectX::Model>m_pModelFall;

	int m_modelTimer;

	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeybord;
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	//	�X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//�J�����̃A���O��
	int m_angle;

	std::unique_ptr<PlayerEffectManager> m_effectManager;

	//�v���C�V�[���J����
	PlaySceneCamera* m_playSceneCamera;
	//�X�e�[�W���[�h�N���X
	StageLoad* m_stageLoad;
	//�v���C���[�̉�]����ۑ����邽�߂̕ϐ�(world�ۑ��p�ϐ�)
	DirectX::SimpleMath::Matrix m_proveWorld;
	//�v���[���[�̏����ʒu
	bool m_resetPosition;
	//���ꂪ���������ǂ����𔻒肷��t���O
	bool m_footholdFlag;
	//���������ǂ������肷��t���O
	bool m_dropFlag;

	//�t�F�[�h���Ƀv���C���[������ł��Ȃ��悤�ɂ��邽�߂ɕK�v
	bool m_textureFlag;

	//�v���C���[�����ꂩ�痎�������ǂ����̃t���O
	bool m_startFootHold;

	//�v���C���[���������Ƃ݂Ȃ��ꂽ���ǂ����̔���
	bool m_fallFlag;
	//�v���C���[�p�̃A���O���ϐ�
	float m_playerAngle;

	//�v���C���[��4�����x���V�e�B
	int m_velocityCount;
	//�x���V�e�B���L���ȃt���O
	bool m_upFlag;
	bool m_underFlag;
	bool m_leftFlag;
	bool m_rightFlag;

	//�L�����������Ă�Œ��A���̗͂����������Ȃ��t���O
	bool m_inputStopFlag;

	//�J�����𓮂����t���O
	//����]
	bool m_cameraLeftFlag;
	//�E��]
	bool m_cameraRightFlag;
	//����]����J�E���g
	int m_cameraLeftCount;
	//�E��]����J�E���g
	int m_cameraRightCount;
	//�v���C���[���������߂ɕK�v�ȃJ�E���g�����Z�b�g����ϐ�
	bool m_velCountReset;

	//�uMiss�v�摜�\�����̃J�E���g
	int m_endDecCount;

	// �G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_fxFactory;

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// SDK�A�j���[�V����
	std::unique_ptr<DX::AnimationSDKMESH> m_animation;

	// �ϊ��z��
	DirectX::ModelBone::TransformArray m_transformArray;

	bool m_keyFlag[4];
};