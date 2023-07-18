#pragma once
#include "ObjectBase.h"
#include <Model.h>
#include "Game/Stage/StageLoad.h"
#include "Game/PlaySceneCamera.h"
//�O���錾
class ObjectManager;

class Floor :public ObjectBase
{
public:
	Floor();
	~Floor();

	void Initialize() override;
	void Update() override;
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) override;

	void SetFootHoldFlag(bool FootHoldFlag) { m_footholdFlag = FootHoldFlag; }
private:
	//�����f��
	std::unique_ptr<DirectX::Model> m_Floor;

	std::vector<bool> m_tileFlag;
	//�v���C���[�̃|�W�V�������󂯎��ϐ�
	DirectX::SimpleMath::Vector3 m_position;

	//�^�C���𓥂ݎn�߂����ǂ����𔻒f����
	bool m_footholdFlag;
};