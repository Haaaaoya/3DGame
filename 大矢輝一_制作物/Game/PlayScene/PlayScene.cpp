/*
	�v���C�V�[��
*/
#include "pch.h"

#include <chrono>
#include <thread>

#include "DeviceResources.h"
#include "../GameMain.h"

#include "PlayScene.h"

#include "Libraries/Adx2/Adx2.h"
#include <Helpers/CueSheet_0.h>
#include "Game/Stage/Stage1.h"

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
PlayScene::PlayScene(int stageNumber) :
	//trans(0.0f),
	m_position{ 0.0f,0.0f,0.0f },
	m_InitialCamPos{ 0.0f,2.0f,5.0f },
	m_CamPos{ 0.0f,0.0f,0.0f },
	m_removeCount(0.0f),
	m_footholdFlag(false),
	m_dropFlag(false),
	m_startFootHold(false),
	m_finishTimer(0.0f),
	m_textureTimer(0.0f),
	m_textureFlag(true),
	m_startTileFlag(false),
	m_lifeCount(5),
	m_lifeFlag(false),
	m_effectCount(0),
	m_clear(false),
	m_cameraLeftFlag(false),
	m_cameraRightFlag(false),
	m_manualFlag(1),
	m_cameraLeftCount(0.0f),
	m_cameraRightCount(0.0f),
	m_pStageBase(nullptr),
	m_nextStage(StageBase::STAGE::STAGE1),
	m_stageChange(false)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �J����
	m_debugCamera = std::make_unique<DebugCamera>();

	//	AABB�{�b�N�X���쐬����
	m_pAABBObjectA = new AABBFor3D();
	m_pAABBObjectB = new AABBFor3D();

	m_stageNumber = stageNumber;

	ResourcesLoad::GetInstance()->PlayTextureLoad();
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
PlayScene::~PlayScene()
{

	delete m_pAABBObjectB;
	m_pAABBObjectB = nullptr;
	delete m_pAABBObjectA;
	m_pAABBObjectA = nullptr;
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void PlayScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//	�X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	DirectX::EffectFactory* factory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//�@�e�N�X�`���̓ǂݎ��p�X�w��
	factory->SetDirectory(L"Resources/Models");
	
	m_pBack = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Snow.cmo",
		*factory
	);
	delete factory;

	m_keybordState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	m_pGeometricPrimitive = DirectX::GeometricPrimitive::CreateBox(pDR->GetD3DDeviceContext(), DirectX::XMFLOAT3(5.0f, 0.5f, 5.0f), true, false);

	//�v���C���[�̏�����
	m_player = std::make_unique<Player>();
	m_player->Initialize();
	//�I���錾�̏�����
	m_endDeclaration = std::make_unique<EndDeclaration>();
	m_endDeclaration->Initialize();

	m_pBack->UpdateEffects([&](DirectX::IEffect* effect)
		{
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights)
			{
				// ���C�g�̉e�����Ȃ���
				lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// �G�~�b�V�����F�𔒂ɐݒ肷��
				basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
			}
		});

	//	�����蔻��{�b�N�X�̏�����
	m_pAABBObjectA->Initialize();
	m_pAABBObjectB->Initialize();

	//	�����蔻��{�b�N�X�̈ʒu�����w��
	m_pAABBObjectA->SetData(DirectX::SimpleMath::Vector3(-6.0f, -8.0f, 2.0f), DirectX::SimpleMath::Vector3(-4.0f, -7.0f, 7.0f));
	m_pAABBObjectB->SetData(DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f), DirectX::SimpleMath::Vector3(5.0f, 5.0f, 5.0f));

	m_rect = { 0,0,400,90 };
	//�L�[�{�[�h
	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	ADX2::GetInstance().Initialize("Resources/Sounds/song.acf", "Resources/Sounds/CueSheet_0.acb");

	ADX2::GetInstance().Play(CRI_CUESHEET_0_������_���Q�[�����y_�_���W����05);

	// �S�̂̉��ʂ̒����ivolume��0.0f�`1.0f�j
	ADX2::GetInstance().SetVolume(0.4f);

	//�v���C�V�[���J�����̐���
	m_playSceneCamera = std::make_unique<PlaySceneCamera>();

	//�I�u�W�F�N�g�̏�����
	m_objectManager = std::make_unique<ObjectManager>();
	m_objectManager->SetPlayer(m_player.get());
	m_objectManager->SetPlaySceneCamera(m_playSceneCamera.get());
	m_objectManager->Initialize(m_stageNumber);

	m_player->SetPlaySceneCamera(m_playSceneCamera.get());

	CreateStage();

	m_pStageBase->SetObjectManager(m_objectManager.get());
	m_pStageBase->SetPlaySceneCamera(m_playSceneCamera.get());
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	int cameraRotation = 3;
	ADX2::GetInstance().Update();

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	m_debugCamera->Update();
	
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);

	m_rotation = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle / 180.0f * DirectX::XM_PI);

	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (m_nextStage != StageBase::STAGE::NONE)
	{
		// �V�[���폜
		DeleteStage();

		// �V�[���쐬
		CreateStage();

		m_tileReset = false;
		SetResetPos(false);
		m_textureFlag = true;
	}

	//�v���C���𒆐S�ɃJ���������悤�ɂ���
	m_CamPos = DirectX::SimpleMath::Vector3::Transform(m_InitialCamPos, m_rotation);
	m_CamPos += DirectX::SimpleMath::Vector3(m_position.x, 2.0f, m_position.z);
	//�J�����̈ʒu��ύX=======================================================
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::A) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraLeftFlag = true;
	}
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::D) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraRightFlag = true;
	}
	//�v���C���[�������Ă�ԁA�J�����͓������Ȃ��悤�ɂ���
	if (m_player->GetInputStopFlag() == true)
	{
		m_cameraLeftFlag = false;
		m_cameraRightFlag = false;
	}

	if (m_cameraLeftFlag == true)
	{
		m_cameraLeftCount++;
		m_angle += 3;
		if (m_cameraLeftCount >= 30)
		{
			m_cameraLeftCount = 0;
			m_cameraLeftFlag = false;
		}
	}

	if (m_cameraRightFlag == true)
	{
		m_cameraRightCount++;
		m_angle -= 3;
		if (m_cameraRightCount >= 30)
		{
			m_cameraRightCount = 0;
			m_cameraRightFlag = false;
		}
	}
	//=========================================================================
	m_InitialCamPos = DirectX::SimpleMath::Vector3(0.0f, 2.0f, 5.0f);

	//�J�������𑗂�A�X�V����
	m_playSceneCamera->SetMoveTarget(m_position);
	m_playSceneCamera->SetCameraPosition(m_CamPos);
	m_playSceneCamera->SetPositon(m_position);
	m_playSceneCamera->MoveCameraSetting();

	
	m_pStageBase->SetRot(m_rotation);
	m_pStageBase->SetPosition(m_position);
	m_nextStage = m_pStageBase->Update(timer);

	//�^�C�������Z�b�g���邩�ǂ����̌��ʂ��󂯎��
	m_tileReset = m_pStageBase->GetResetPos();
	m_removeCount = m_pStageBase->GetRemoveCount();
	//�I������t���O���X�e�[�W�ɑ���
	m_finish = m_pStageBase->GetFinish();

	m_startFootHold = m_pStageBase->GetStartFootHold();
	//�����^�C���𓥂񂾎��ɗ��t���O��
	//�v���C���[�N���X�ɑ���
	m_player->SetVelCountReset(m_objectManager->GetAllResetFlag());
	//���������ǂ������肷��t���O(m_fallFlag)��
	//�v���C���[�ɑ���
	m_player->SetFallFlag(m_pStageBase->GetFallFlag());

	switch (m_stageNumber)
	{
	case ONE:
		break;
	case TWO:
		//�X�e�[�W���ς��̂ŁA�摜�̐؂蔲���ʒu��ύX
		//m_rect = { 0,90,400,180 };
		break;
	case THREE:
		//�X�e�[�W���ς��̂ŁA�摜�̐؂蔲���ʒu��ύX
		//m_rect = { 0,180,400,270 };
		break;
	case FOUR:
		//�X�e�[�W���ς��̂ŁA�摜�̐؂蔲���ʒu��ύX
		m_rect = { 0,270,400,360 };
		break;
	case FIVE:
		//�X�e�[�W���ς��̂ŁA�摜�̐؂蔲���ʒu��ύX
		m_rect = { 0,360,400,450 };
		break;
	}
	
	//�摜�̓����x��M��ϐ���max��min�����߂Ă���
	//=====================================================================
	if (m_textureTimer >= 1.0f)
	{
		m_textureTimer = 1.0f;
	}

	if (m_textureTimer <= 0.0f)
	{
		m_textureTimer = 0.0f;
	}
	//=====================================================================
	
	//�t�F�[�h�C���A�E�g�̏���
	//=====================================================================
	if (m_finish == true)
	{
		m_finishTimer += 1 / 60.0f;
		m_textureFlag = false;
	}

	//�I�����鎞�A�v���C���[������ł��Ȃ��悤�ɂ���
	
	//�v���C���[�N���X��bool�^�ϐ��𑗂�
	m_player->SetStartFootHold(m_startFootHold);
	//=====================================================================

	//�L�����̈ړ�=============================================================
	
	m_position = m_player->GetPosition();
	m_player->SetTextureFlag(m_textureFlag);
	//����𓥂�ł��邩�ǂ����̃t���O��Player�N���X�ɑ���
	m_player->SetFootholdFlag(m_footholdFlag);

	//�}�l�[�W���[�Ƀv���C���[�̈ʒu��n��
	m_objectManager->SetPosition(m_position);
	//�X�V
	m_objectManager->Update();

	m_player->Update(timer);
	//�v���C���[�����������Ƀt�F�[�h�̏������s��
	if (m_position.y <= -5.0f && m_position.y >= -6.0f)
	{
		m_textureFlag = false;
	}
	if (m_position.y < -40.0f)
	{
		m_textureFlag = true;
	}
	//�Ăѓ�����悤�ɂȂ�^�C�}�[��1.0�𒴂����烉�C�t��1����
	if (m_objectManager->GetRemoveCount() >= 1.0f)
	{
		m_lifeCount--;
	}
	//���C�t��0�����ɂȂ�����Q�[���I�[�o�[
	if (m_lifeCount < 0)
	{
		m_textureFlag = false;
		return GAME_SCENE::RESULT;
	}
	//�Q�[���̐������̐؂�ւ�
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		switch (m_manualFlag)
		{
		case 0:
			m_manualFlag = 1;
			break;
		case 1:
			m_manualFlag = 2;
			break;
		case 2:
			m_manualFlag = 0;
			break;
		}
	}
	//�I�u�W�F�N�g��S�ă��Z�b�g������
	if (m_objectManager->GetAllResetFlag() == true)
	{
		m_endDeclaration->Update(timer);
	}
	//�N���A������󂯎�������A���U���g�V�[����
	if (m_objectManager->GetClearFlag() == true)
	{
		m_finishTimer += 0.1f;
		m_clear = true;
	}
	//�I���̃^�C�}�[����莞�Ԍo������
	if (m_finishTimer >= 4.0f)
	{
		return GAME_SCENE::RESULT;
	}

	//�t�F�[�h�̏���
	Fade();

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	//	�ϊ��s��̐錾(���[���h�A�r���[�A�ˉe(�v���W�F�N�V����))
	DirectX::SimpleMath::Matrix world;

	DirectX::SimpleMath::Matrix celestialSphereWorld, view1;

	//	���[���h�s��̏�����(�P�ʍs��)
	world = DirectX::SimpleMath::Matrix::Identity;
	/*view = m_pDebugCamera->GetViewMatrix();*/							//	�f�o�b�O�J�������g��

	
	// �R�[���o�b�N�֐���n�����߂̕ϐ��̐ݒ�
	std::function<void()> func = std::bind(&PlayScene::SetState, this);

	//�V���̃e�N�X�`��
	m_pBack->Draw(context, *m_commonState.get(), celestialSphereWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());

	m_objectManager->Draw(m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());

	//�v���C���[�`��
	m_player->Draw();

	//�X�e�[�W�̕`��
	m_pStageBase->Draw();
	m_dropFlag = m_pStageBase->GetDropFlag();
	m_player->SetDropFlag(m_dropFlag);


	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());
	//�X�e�[�W���\��
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureStage).Get(), DirectX::SimpleMath::Vector2(0, 0), &m_rect, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 1.0f);

	for (int i = 0; i < m_lifeCount; i++)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureLife).Get(), DirectX::SimpleMath::Vector2(350 + (i * 100), 0), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.2f);
	}

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureManual).Get(), DirectX::SimpleMath::Vector2(1050, 0), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.1f);
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureSpacekey).Get(), DirectX::SimpleMath::Vector2(900, 0), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.5f);

	if (m_manualFlag == 1)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureManual1Page).Get(), DirectX::SimpleMath::Vector2(120, 70), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 1.0f);
	}
	else if(m_manualFlag == 2)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureManual2Page).Get(), DirectX::SimpleMath::Vector2(120, 70), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 1.0f);
	}

	//�t�F�[�h�p�̉摜�\��
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureBlack).Get(), DirectX::SimpleMath::Vector2(0, 0), 0, { 1.0f,1.0f,1.0f,m_textureTimer }, 0.0f, DirectX::g_XMZero, 3.0f);

	m_spriteBatch->End();

	m_endDeclaration->MissDraw();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void PlayScene::Finalize()
{
	ADX2::GetInstance().Finalize();
}

void PlayScene::LoadResources()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();

	std::this_thread::sleep_for(std::chrono::seconds(1));
}
void PlayScene::Fade()
{
	if (m_textureFlag == false)
	{
		m_textureTimer += 0.01f;
	}
	else
	{
		m_textureTimer -= 0.01f;
	}
}

void PlayScene::SetState()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	// �J�����O���Ȃ��ݒ�
	context->RSSetState(m_commonState->CullNone());
}
bool PlayScene::GetClearFlag()
{
	return m_clear;
}

int PlayScene::GetStageNum()
{
	return 0;
}

void PlayScene::CreateStage()
{
	//�X�e�[�W���쐬����Ă���Ƃ��͏������Ȃ�
	if (m_pStageBase != nullptr)
	{
		return;
	}

	//���̃X�e�[�W�쐬
	switch (m_nextStage)
	{
	case StageBase::STAGE::STAGE1:
		m_pStageBase = std::make_unique<Stage1>();
		break;
	default:
		// ��O�Ȃ̂ŏ����𒆒f
		return;
	}
	m_pStageBase->Initialize();
	
	m_nextStage = StageBase::STAGE::NONE;
}

void PlayScene::DeleteStage()
{
	// �X�e�[�W���쐬����Ă��Ȃ���Ώ������Ȃ�
	if (m_pStageBase == nullptr)
	{
		return;
	}

	// ���X�e�[�W�̏I������
	m_pStageBase->Finalize();

	// ���X�e�[�W�̍폜
	m_pStageBase.reset();
}