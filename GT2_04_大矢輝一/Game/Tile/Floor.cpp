#include "pch.h"
#include "Floor.h"
#include "DeviceResources.h"
#include <WICTextureLoader.h>
#include <Effects.h>

Floor::Floor():
	m_footholdFlag(false)
{
}

Floor::~Floor()
{

}

void Floor::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//�����f���̃e�N�X�`���̓ǂݎ��p�X�w��
	std::unique_ptr<DirectX::EffectFactory> tilefactory = std::make_unique<DirectX::EffectFactory>(pDR->GetD3DDevice());
	tilefactory->SetDirectory(L"Resources/Models");
	//���̃��f���ǂݍ���
	m_Floor = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Floor.cmo",
		*tilefactory
	);
	tilefactory.reset();
}

void Floor::Update()
{

}

void Floor::Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);
	//���̏����ʒu
	static const DirectX::SimpleMath::Vector3 floorInitialPosition =
		DirectX::SimpleMath::Vector3(0.21f, 0.25f, 0.21f);
	//���̃��[���h���W
	DirectX::SimpleMath::Matrix floorWorld;
	//�v���C���[���^�C���𓥂ݎn�߂Ă��Ȃ���Ε`�������
	if (m_footholdFlag == false)
	{
		for (int i = 0; i < 8; i++)
		{
			floorWorld = DirectX::SimpleMath::Matrix::Identity;
			//���̑傫����ݒ�
			floorWorld *= DirectX::SimpleMath::Matrix::CreateScale(floorInitialPosition);
			//���̈ʒu��ݒ�
			floorWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 2.5f * i);
			for (int j = 0; j < 8; j++)
			{
				floorWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(2.5f, 0.0f, 0.0f);

				if (GetStageLoad()->GetStageData(i, j) == 2)
				{
					//�t���A�̕`��
					m_Floor->Draw(context, *GetCommonState(), floorWorld, GetPlaySceneCamera()->GetView(), GetPlaySceneCamera()->GetProjection());
				}
			}
		}
	}
}