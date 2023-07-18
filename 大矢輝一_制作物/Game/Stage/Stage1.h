#pragma once

#include<GeometricPrimitive.h>
#include "Game/AABBFor3D.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>

#include "Game/GameMain.h"

#include <CommonStates.h>
#include<Model.h>

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

#include "Game/Stage/StageBase.h"
#include "Game/PlaySceneCamera.h"

class Stage1:public StageBase
{
private:
	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;

	AABBFor3D* m_pAABBObjectA;
	std::unique_ptr<AABBFor3D> m_pAABBObjectB;

	// �W�I���g���v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive> m_pGeometricPrimitive;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keybordStateTracker;
public:
	// �R���X�g���N�^
	Stage1();

	// �f�X�g���N�^
	~Stage1() override;

	// ������
	void Initialize() override;

	// �X�V
	STAGE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	bool AABB(DirectX::SimpleMath::Vector3 a1, DirectX::SimpleMath::Vector3 a2,
		DirectX::SimpleMath::Vector3 b1, DirectX::SimpleMath::Vector3 b2);

	////�^�C�������Z�b�g����ϐ��𑗂�֐�
	//bool GetResetPos();
	////�v���C���[���^�C���ɏ�������ǂ������肷��ϐ��𑗂�֐�
	//bool GetFootholdFlag();
	////�v���C���[�����������ǂ������肷��ϐ��𑗂�֐�
	//bool GetDropFlag();
	////���̃X�e�[�W���I��������ǂ������肷��ϐ��𑗂�֐�
	//bool GetFinish();
	////�S�Ẵ^�C�������Z�b�g�������ǂ������Q�b�g����ׂ̊֐�
	//bool GetAllResetFlag() { return m_allResetFlag; }
	////���������ǂ������Q�b�g����ׂ̊֐�
	//bool GetFallFlag() { return m_fallFlag; }

	////���ꂩ�痣��Ă��邩�ǂ����̃t���O���Q�b�g����֐�
	//bool GetStartFootHold();

	//float GetRemoveCount();
	////�Q�[���I���̃^�C�}�[���̊֐�
	//float FinishTimer();
};