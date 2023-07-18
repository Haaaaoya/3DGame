#include "pch.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager():
	m_position(DirectX::SimpleMath::Vector3::Zero)
{
}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Initialize(int stageNumber)
{
	m_stageLoad = std::make_unique<StageLoad>();

	//�X�e�[�W�f�[�^�����Ă���
	std::vector<const char*> fileName{
		{"Resources/StageData/StageData1.csv"},
		{"Resources/StageData/StageData2.csv"},
		{"Resources/StageData/StageData3.csv"},
		{"Resources/StageData/StageData4.csv"},
		{"Resources/StageData/StageData5.csv"},
		{"Resources/StageData/StageCreate.csv"},
	};
	//���������ƂɃf�[�^��ǂݍ���
	m_stageLoad->LoadStageData(fileName[stageNumber - 1]);
	//�^�C���N���X�ƃt���A�N���X��
	//�J�����N���X�ƃX�e�[�W���[�h�N���X�̃|�C���^�𑗂�
	m_normalTile = std::make_unique<NormalTile>();
	m_normalTile->SetPlaySceneCamera(m_playSceneCamera);
	m_normalTile->SetStageLoad(m_stageLoad.get());
	//������
	m_normalTile->Initialize();
	m_floor = std::make_unique<Floor>();
	m_floor->SetPlaySceneCamera(m_playSceneCamera);
	m_floor->SetStageLoad(m_stageLoad.get());
	//������
	m_floor->Initialize();
	m_player->SetStageLoad(m_stageLoad.get());
}

void ObjectManager::Update()
{
	//�^�C���N���X�Ƀv���C���[�̈ʒu�𑗂�
	m_normalTile->SetPosition(m_position);
	//�^�C���N���X�̍X�V
	m_normalTile->Update();
	//���N���X�Ƀv���C���[�̈ʒu�𑗂�
	m_floor->SetPosition(m_position);
	//�^�C���N���X����^�C���ɏ�������ǂ�����������
	m_floor->SetFootHoldFlag(m_normalTile->GetFootHoldFlag());
	m_player->SetFootholdFlag(m_normalTile->GetFootHoldFlag());
	//�^�C�������Z�b�g���ꂽ���ǂ���
	m_player->SetResetPos(m_normalTile->GetTileReset());
}

void ObjectManager::Draw(const DirectX::SimpleMath::Matrix& view,const DirectX::SimpleMath::Matrix& projection)
{
	m_normalTile->Draw(view,projection);
	m_floor->Draw(view, projection);
}