#include "pch.h"
#include "StageBase.h"
#include "DeviceResources.h"
#include <WICTextureLoader.h>
#include "Libraries/Adx2/Adx2.h"
#include <Helpers/CueSheet_0.h>

// ���O��Ԃ̗��p
using namespace DirectX;
using namespace DirectX::SimpleMath;

StageBase::StageBase() :
	m_tileReset(false),
	m_removeCount(0.0f),
	m_InitialCamPos{ 0.0f,2.0f,5.0f },
	m_CamPos{ 0.0f,0.0f,0.0f },
	m_dropFlag(false),
	m_finish(false),
	m_footholdFlag(false),
	m_finishTimer(0.0f),
	m_allResetFlag(false),
	m_cameraMode(false),
	m_allResetCount(0),
	m_fallFlag(false),
	m_stageChange(false),
	m_startFootHold(false)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//	�X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//���񂾌�̃^�C���̃e�N�X�`���̓ǂݎ��p�X�w��
	DirectX::EffectFactory* AfterTilefactory = new EffectFactory(pDR->GetD3DDevice());
	AfterTilefactory->SetDirectory(L"Resources/Models");

	m_pAfterTile = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/AfterTile.cmo",
		*AfterTilefactory
	);
	delete AfterTilefactory;

	//���ޑO�̃^�C���̃e�N�X�`���̓ǂݎ��p�X�w��
	EffectFactory* Tilefactory = new EffectFactory(pDR->GetD3DDevice());
	Tilefactory->SetDirectory(L"Resources/Models");

	m_pTile = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Tile.cmo",
		*Tilefactory
	);
	delete Tilefactory;

	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/black.png",
		nullptr,
		m_blackTexture.ReleaseAndGetAddressOf()
	);

	ADX2::GetInstance().Initialize("Resources/Sounds/song.acf", "Resources/Sounds/CueSheet_0.acb");

	m_objectBase = std::make_unique<ObjectBase>();
}

StageBase::~StageBase()
{
}

void StageBase::Initialize()
{}

StageBase::STAGE StageBase::Update(const DX::StepTimer& timer)
{
	return STAGE::NONE;
}

void StageBase::Draw()
{}

void StageBase::Finalize()
{}

void StageBase::TileReset()
{
	for (int i = 0; i < 28; i++)
	{
		if (m_tileFlag[i] == true && m_tileCount[i] == 0)
		{
			m_tileCount[i] = 1;
		}
		if (m_tileFlag[i] == false && m_tileCount[i] == 1)
		{
			m_tileCount[i] = 2;
		}
		//������x�^�C���𓥂񂾎�
		if (m_tileFlag[i] == true && m_tileCount[i] == 2)
		{
			m_allResetFlag = true;
		}
	}
}

void StageBase::CreateEffect()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_eye = DirectX::SimpleMath::Vector3(
		0.0f, 2.0f, m_position.z + 4
	);
	m_target = DirectX::SimpleMath::Vector3(
		1.0f, 1.0f, 0.0f
	);
	m_up = DirectX::SimpleMath::Vector3(
		0.0f, 1.0f, 0.0f
	);

	//�����_���̍쐬
	m_renderer = EffekseerRendererDX11::Renderer::Create(device, context, 8000);

	//�}�l�[�W���̍쐬
	m_manager = Effekseer::Manager::Create(8000);

	//�`�惂�W���[���̐ݒ�
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());

	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());

	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());

	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());

	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());

	//�e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ�
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());

	m_manager->SetModelLoader(m_renderer->CreateModelLoader());

	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());

	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	//�`��̎n�_�ʒu���m�F
	Effekseer::Vector3D pos = Effekseer::Vector3D(m_eye.x, m_eye.y, m_eye.z);

	//�����_���̎ˉe�s���ݒ�
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);
	m_renderer->SetProjectionMatrix(
		Effekseer::Matrix44().PerspectiveFovRH(
			90.0f / 180.0f * 3.14f,
			width / height,
			1.0f,
			500.0f)
	);

	//�����_���̃J�����s���ݒ�
	m_renderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtRH(
			pos,
			Effekseer::Vector3D(m_target.x, m_target.y, m_target.z),
			Effekseer::Vector3D(0.0f, 1.0f, 0.0f)
		)
	);

	//�G�t�F�N�g�̓ǂݍ���
	m_effect = Effekseer::Effect::Create(m_manager, (const EFK_CHAR*)L"Resources/Effekseer/aqua.efk");

	//�G�t�F�N�g�n���h���̏�����
	m_handle = 0;
}