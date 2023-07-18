#pragma once
#include "ObjectBase.h"
#include <Model.h>
#include "Game/Stage/StageLoad.h"
#include "Game/PlaySceneCamera.h"
//�O���錾
class ObjectManager;

class NormalTile:public ObjectBase
{
public:
	enum Models
	{
		Tile,
		AfterTile,

		ModelNum
	};

	NormalTile();
	~NormalTile();

	void Initialize() override;
	void Update() override;
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) override;
	//�^�C���̏���
	void TileProcess();

	//�^�C���̃��Z�b�g��m�点��֐�
	bool GetTileReset() { return m_tileReset; }
	//get
	//�v���C���[���^�C���ɏ�������ǂ������肷��ϐ����Q�b�g����֐�
	bool GetFootHoldFlag() { return m_footholdFlag; }
	//�S�Ẵ^�C�������Z�b�g�������ǂ������Q�b�g����ׂ̊֐�
	bool GetAllResetFlag() { return m_allResetFlag; }
	//void SetObjectManager(ObjectManager* objectManager) { m_objectManager = objectManager; }

	float GetRemoveCount() { return m_removeCount; }

	//�N���A�������ǂ������Q�b�g����֐�
	bool GetClearFlag(){return m_clearFlag;}
private:
	// ���f�����e�[�u��
	static const wchar_t* ModelNameTable[ModelNum];

	// ���f��
	std::unique_ptr<DirectX::Model> m_models[ModelNum];
	//�v���C���[�̃|�W�V�������󂯎��ϐ�
	DirectX::SimpleMath::Vector3 m_position;

	//�^�C���𓥂ݎn�߂����ǂ����𔻒f����
	bool m_footholdFlag;
	//�v���C���[�̌��ݒn������o���ϐ����󂯎��
	int m_indexX;
	int m_indexY;

	bool m_tileFlag[8][8];
	int m_tileNum[8][8];

	//�^�C�������Z�b�g����܂ł̃J�E���g
	int m_allResetCount;

	//�^�C����S�ă��Z�b�g����t���O
	bool m_allResetFlag;

	//�G�t�F�N�g��\������Ƃ��Ɏg��
	int m_effectCount[8][8];

	//�Ăѓ����o����܂ł̎���
	float m_removeCount;

	bool m_tileReset;

	//���������ǂ������肷��t���O
	bool m_fallFlag;

	//�^�C���̃}�X�𐔂���ϐ�
	int m_tileCount;
	//���܂ꂽ�^�C���̐��𐔂���ϐ�
	int m_tileNumCount;

	//�N���A�������ǂ����̃t���O
	bool m_clearFlag;
};