#pragma once
#include<Model.h>
#include<GeometricPrimitive.h>
#include "Game/AABBFor3D.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include "Game/Stage/StageLoad.h"
#include "Game/GameMain.h"
#include "Game/Tile/ObjectBase.h"
#include "Game/PlaySceneCamera.h"
#include "Game/Tile/ObjectManager.h"

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

class StageBase
{
public:
	enum class STAGE : int
	{
		NONE,

		STAGE1,
		STAGE2,
		STAGE3,
		STAGE4,
		STAGE5
	};
public:
	//getter,setter�֐�
	//========================================================================
	void SetRot(const DirectX::SimpleMath::Matrix& rot) { m_rotation = rot; }

	void SetCameraPos(const DirectX::SimpleMath::Vector3& CamPos) { m_CamPos = CamPos; }

	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }

	void SetPlaySceneCamera(PlaySceneCamera* playSceneCamera) { m_playSceneCamera = playSceneCamera; }

	void SetObjectManager(ObjectManager* objectManager) { m_objectManager = objectManager; }
	//ObjectBase* GetObjectBase() { return m_objectBase.get(); }

	//�^�C�������Z�b�g����ϐ��𑗂�֐�
	bool GetResetPos() { return m_tileReset; }
	//�v���C���[���^�C���ɏ�������ǂ������肷��ϐ��𑗂�֐�
	bool GetFootholdFlag() { return m_footholdFlag; }
	//�v���C���[�����������ǂ������肷��ϐ��𑗂�֐�
	bool GetDropFlag() { return m_dropFlag; }
	//���̃X�e�[�W���I��������ǂ������肷��ϐ��𑗂�֐�
	bool GetFinish() { return m_finish; }
	//�S�Ẵ^�C�������Z�b�g�������ǂ������Q�b�g����ׂ̊֐�
	bool GetAllResetFlag() { return m_allResetFlag; }
	//�X�e�[�W�����������邩�ǂ����̕ϐ����Z�b�g����ׂ̊֐�
	void SetStartTile(const bool& startTileFlag) { m_startTileFlag = startTileFlag; }
	//���������ǂ������Q�b�g����ׂ̊֐�
	bool GetFallFlag() { return m_fallFlag; }

	//���ꂩ�痣��Ă��邩�ǂ����̃t���O���Q�b�g����֐�
	bool GetStartFootHold() { return m_startFootHold; }

	float GetRemoveCount() { return m_removeCount; }

	void SetStageChange(const bool& stageChange) { m_stageChange = stageChange; }
	//========================================================================

	StageBase();
	virtual ~StageBase();

	virtual void Initialize();

	virtual STAGE Update(const DX::StepTimer& timer);
	// �`��
	virtual void Draw();

	// �I������
	virtual void Finalize();

	void TileReset();

	float FinishTimer(){ return m_finishTimer; }
	
	//�G�t�F�N�g�̏����ݒ�
	void CreateEffect();

protected:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>                      m_commonState;

	AABBFor3D* m_pAABBObjectA;
	AABBFor3D* m_pAABBObjectB;

	// �W�I���g���v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive> m_pGeometricPrimitive;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	//���ޑO�̏�Ԃ̃^�C���̃��f���ǂݍ��ݗp�ϐ�
	std::unique_ptr<DirectX::Model> m_pTile;
	//���񂾌�̏�Ԃ̃^�C���̃��f���ǂݍ��ݗp�ϐ�
	std::unique_ptr<DirectX::Model> m_pAfterTile;

	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;

	std::unique_ptr<StageLoad> m_stageLoad;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blackTexture;

	//�J�����̈ʒu���Ȃ�
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Vector3 m_target;
	DirectX::SimpleMath::Vector3 m_up;

	std::vector<int> m_tileCount;

	std::vector<bool> m_tileFlag;
	bool m_tileReset;

	//�^�C���𓥂ޑO�ɑ��ꂩ�痎�������ǂ����Ŏg���t���O
	//true������ɏ���Ă���Afalse�����ꂩ�痎��������Ƃ���
	//�^�C���𓥂ݎn�߂��炱�̃t���O�͎g��Ȃ�
	bool m_startFootHold;

	//�^�C���𓥂ݎn�߂����ǂ����𔻒f����
	bool m_footholdFlag;
	//�v���C���[�����������ǂ����𔻒f����
	bool m_dropFlag;

	bool m_finish;

	//�X�e�[�W���؂�ւ��ہA�^�C�������ɓ���ł���Ƃ����o�O��
	//�������߂����ɕK�v�ȃt���O
	bool m_startTileFlag;

	//�t�F�[�h�A�E�g����������܂ŃQ�[�����I��点�Ȃ��ׂ̃^�C�}�[
	float m_finishTimer;

	//�J�����̈ʒu��؂�ւ���t���O
	bool m_cameraMode;

	//�^�C����S�ă��Z�b�g����t���O
	bool m_allResetFlag;

	//�Ăѓ����o����܂ł̎���
	float m_removeCount;
	//�v���C���[�̃|�W�V�������󂯎�邽�߂̕ϐ�
	DirectX::SimpleMath::Vector3 m_position;

	DirectX::SimpleMath::Vector3 m_InitialCamPos;
	DirectX::SimpleMath::Vector3 m_CamPos;
	DirectX::SimpleMath::Matrix m_rotation;

	//Effekseer�Ŏg�p����p�[�c�������o�ϐ��ɒǉ�����
	//�����_��
	EffekseerRendererDX11::RendererRef m_renderer;
	//�}�l�[�W��
	Effekseer::ManagerRef m_manager;
	//�G�t�F�N�g
	Effekseer::EffectRef m_effect;
	//�n���h��
	Effekseer::Handle m_handle;

	//�G�t�F�N�g��\������Ƃ��Ɏg��
	std::vector<int> m_effectCount;

	//�^�C�������Z�b�g����܂ł̃J�E���g
	int m_allResetCount;

	//���������ǂ������肷��t���O
	bool m_fallFlag;

	int m_tileResetCount;

	//�X�e�[�W�؂�ւ��̃t���O���󂯎��ϐ�
	bool m_stageChange;

	//�X�e�[�W�f�[�^���i�[����ϐ�
	int m_stageData[7][4];

	//CSV�̍s�����󂯎��ϐ�
	DirectX::SimpleMath::Vector2 m_stagePos;

	std::unique_ptr<ObjectBase> m_objectBase;

	PlaySceneCamera* m_playSceneCamera;

	ObjectManager* m_objectManager;
};