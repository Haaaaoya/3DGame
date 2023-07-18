#include "pch.h"
#include "NormalTile.h"
#include "DeviceResources.h"
#include <WICTextureLoader.h>
#include <Effects.h>

const wchar_t* NormalTile::ModelNameTable[NormalTile::ModelNum] =
{
	L"Tile.cmo",
	L"AfterTile.cmo"
};

NormalTile::NormalTile():
	m_footholdFlag (false),
	m_indexX(0),
	m_indexY(0),
	m_allResetFlag(false),
	m_removeCount(0.0f),
	m_fallFlag(false),
	m_allResetCount(0),
	m_tileReset(false),
	m_tileCount(0),
	m_tileNumCount(0),
	m_clearFlag(false)
{
	for (int i = 0; i < 8 ; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_tileFlag[i][j] = false;
			m_tileNum[i][j] = 0;
			m_effectCount[i][j] = 0;
		}
	}
}

NormalTile::~NormalTile()
{

}

void NormalTile::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	for (int i = 0; i < ModelNum; i++)
	{
		//���ޑO�̃^�C���̃e�N�X�`���̓ǂݎ��p�X�w��
		DirectX::EffectFactory* Tilefactory = new DirectX::EffectFactory(pDR->GetD3DDevice());
		Tilefactory->SetDirectory(L"Resources/Models");

		std::wstring wstr = L"Resources/Models/";
		wstr += ModelNameTable[i];
		m_models[i]= DirectX::Model::CreateFromCMO(
			pDR->GetD3DDevice(),
			wstr.c_str(),
			*Tilefactory
		);
		delete Tilefactory;
	}
}

void NormalTile::Update()
{
	m_position = GetPosition();
	//�v���C���[�̃|�W�V�����̏����_�ȉ���؂�̂Ă�
	m_position.x = round(m_position.x * 10) / 10;
	m_position.z = round(m_position.z * 10) / 10;

	//�v���C���[�̌��ݒn������o���ϐ�
	int indexX = static_cast<int>((m_position.x + 3.f + 1.25f) / 2.5f);
	int indexY = static_cast<int>((m_position.z + 1.25f) / 2.5f);

	//�^�C���𓥂񂾏ꍇ
	if (GetStageLoad()->GetStageData(indexY, indexX) == 1)
	{
		m_footholdFlag = true;
	}
	//�����ʒu�ɖ߂���1�b�o�ƃQ�[�����ĊJ�����
	if (m_removeCount >= 1.0f)
	{
		m_allResetFlag = false;
		m_tileReset = false;
		m_removeCount = 0.0f;
		m_allResetCount = 0;
		m_fallFlag = false;
	}

	//����������
	if (m_position.y < -40.0f)
	{
		m_fallFlag = true;
	}
	//�^�C���̏���
	TileProcess();

	//�����^�C���𓥂񂾎��̃��Z�b�g����
	if (m_allResetFlag == true)
	{
		//�Q�[���X�^�[�g���ɖ߂��܂ł̃J�E���g�_�E���J�n
		m_allResetCount++;
		//�^�C���֌W�̃t���O��^�C���𓥂񂾎��ɃG�t�F�N�g��
		//����������z��ϐ���������Ԃɖ߂�
		if (m_allResetCount > 240)
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					m_tileFlag[i][j] = false;
					m_tileNum[i][j] = 0;
					m_effectCount[i][j] = 0;
				}
			}
			//�^�C�������Z�b�g���ꂽ
			m_tileReset = true;
			//�v���C���[�͏��̏�ɖ߂����̂ŁA�^�C���ɏ��������
			//���t���O�͉�����
			m_footholdFlag = false;
			//�v���C���[���Ăѓ�����܂ł̃J�E���g�J�n
			m_removeCount += 1.0f / 60;
		}
	}
	//�v���C���[���������ꍇ
	else if (m_fallFlag == true)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				m_tileFlag[i][j] = false;
				m_tileNum[i][j] = 0;
				m_effectCount[i][j] = 0;
			}
		}
		//�^�C�������Z�b�g���ꂽ
		m_tileReset = true;
		//�v���C���[�͏��̏�ɖ߂����̂ŁA�^�C���ɏ��������
		//���t���O�͉�����
		m_footholdFlag = false;
		//�v���C���[���Ăѓ�����܂ł̃J�E���g�J�n
		m_removeCount += 1.0f / 60;
	}
}

void NormalTile::Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);
	//���̏����ʒu
	static const DirectX::SimpleMath::Vector3 TileInitialPosition =
		DirectX::SimpleMath::Vector3(0.21f, 0.25f, 0.21f);

	//�J�������̍X�V
	DirectX::SimpleMath::Matrix tileWorld;
	DirectX::SimpleMath::Matrix afterTileWorld;

	//�^�C���̐��Ɠ��܂ꂽ�^�C���̐���������
	m_tileCount = 0;
	m_tileNumCount = 0;
	for (int i = 0; i < 8; i++)
	{
		tileWorld = DirectX::SimpleMath::Matrix::Identity;
		//�^�C���̑傫����ݒ�
		tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(TileInitialPosition);
		//�^�C���̈ʒu��ݒ�
		tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 2.5f * i);
		for (int j = 0; j < 8; j++)
		{
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(2.5f, 0.0f, 0.0f);

			//�v���C���[�̌��ݒn������o���ϐ�
			int indexX = static_cast<int>((m_position.x + 3.f) / 2.5f);
			int indexY = static_cast<int>(m_position.z / 2.5f);
			//CSV�̃f�[�^��1�̎�
			if (GetStageLoad()->GetStageData(i, j) == 1)
			{
				//�^�C���̐��𐔂���
				m_tileCount++;
				//�^�C���̕`��
				m_models[Tile]->Draw(context, *GetCommonState(), tileWorld, GetPlaySceneCamera()->GetView(), GetPlaySceneCamera()->GetProjection());

				if(GetStageLoad()->GetStageData(indexY, indexX) == 1)
				{
					//CSV���c8,��8
					m_tileFlag[indexY][indexX] = true;
				}
			}
			//�^�C���𓥂ނƐF���ς��
			if (m_tileNum[i][j] == 1 || m_tileNum[i][j] == 2)
			{
				//���񂾌�̃^�C���̃��[���h���W
				afterTileWorld = DirectX::SimpleMath::Matrix::Identity;
				afterTileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
				afterTileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-3.f + 2.5f * j, 0.0f, 2.5f * i);
				//���񂾌�̃^�C���̕`��
				m_models[AfterTile]->Draw(context, *GetCommonState(), afterTileWorld, GetPlaySceneCamera()->GetView(), GetPlaySceneCamera()->GetProjection());
				m_tileFlag[i][j] = false;
				//���񂾃^�C���̐��𐔂���
				m_tileNumCount++;
			}
		}
	}
	//�`�悳��Ă�^�C���̐��Ɠ��񂾃^�C���̐�����v������
	//�N���A�����ɂȂ�
	//���̏������͓��񂾃^�C���𐔂���ϐ��ƃ^�C���̃}�X�𐔂���ϐ���
	//�X�V���������Ƃɏ����K�v������
	if (m_tileCount == m_tileNumCount)
	{
		m_clearFlag = true;
	}
}

void NormalTile::TileProcess()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//�^�C�������܂ꂽ�����n�߂����񂾃^�C���̏ꍇ
			if (m_tileFlag[i][j] == true && m_tileNum[i][j] == 0)
			{
				m_tileNum[i][j] = 1;
			}
			//��x���񂾃^�C�����痣�ꂽ��
			if (m_tileFlag[i][j] == false && m_tileNum[i][j] == 1)
			{
				m_tileNum[i][j] = 2;
			}
			//������x�����^�C���𓥂񂾎�
			if (m_tileFlag[i][j] == true && m_tileNum[i][j] == 2)
			{
				m_allResetFlag = true;
			}
		}
		
	}
}