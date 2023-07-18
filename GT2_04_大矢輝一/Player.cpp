#include "pch.h"
#include "Player.h"
#include "DeviceResources.h"

#include "Game/GameMain.h"
#include <Effects.h>
#include <SimpleMath.h>

// ���O��Ԃ̗��p
using namespace DirectX;
using namespace DirectX::SimpleMath;

#define PI        (3.14f)
#define DROPSPEED (0.2f)
#define MAX_ANGLE (3)

Player::Player():
	m_position(DirectX::SimpleMath::Vector3(-3.f, 0.5f, 0.f)),
	m_initialPosition(-3.f, 0.5f, 0.f),
	m_resetPosition(false),
	m_footholdFlag(false),
	m_dropFlag(false),
	m_startFootHold(false),
	m_modelTimer(0),
	m_fallFlag(false),
	m_playerAngle(0.0f),
	m_velocityCount(0),
	m_upFlag(false),
	m_underFlag(false),
	m_leftFlag(false),
	m_rightFlag(false),
	m_cameraLeftFlag(false),
	m_cameraRightFlag(false),
	m_cameraLeftCount(0.0f),
	m_cameraRightCount(0.0f),
	m_angle(0),
	m_inputStopFlag(false),
	m_velCountReset(false),
	m_endDecCount(0),
	m_keyFlag{}
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_pDebugCamera = new DebugCamera();
}

Player::~Player()
{
	if (m_pDebugCamera != nullptr)
	{
		delete m_pDebugCamera;
		m_pDebugCamera = nullptr;
	}
}

void Player::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	EffectFactory* factory = new EffectFactory(pDR->GetD3DDevice());

	//�@�e�N�X�`���̓ǂݎ��p�X�w��
	factory->SetDirectory(L"Resources/Models");

	//�@�@�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModel = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/normal.cmo",
		*factory
	);

	delete factory;

	EffectFactory* Rightfactory = new EffectFactory(pDR->GetD3DDevice());

	//�@�e�N�X�`���̓ǂݎ��p�X�w��
	Rightfactory->SetDirectory(L"Resources/Models");

	//�@�@�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModelRight = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/rightLeg.cmo",
		*Rightfactory
	);

	delete Rightfactory;

	EffectFactory* Leftfactory = new EffectFactory(pDR->GetD3DDevice());

	//�@�e�N�X�`���̓ǂݎ��p�X�w��
	Leftfactory->SetDirectory(L"Resources/Models");

	//�@�@�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModelLeft = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/leftLeg.cmo",
		*Leftfactory
	);

	delete Leftfactory;

	EffectFactory* Fallfactory = new EffectFactory(pDR->GetD3DDevice());

	//�@�e�N�X�`���̓ǂݎ��p�X�w��
	Fallfactory->SetDirectory(L"Resources/Models");

	//�@�@�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModelFall = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/fallHuman.cmo",
		*Fallfactory
	);

	delete Fallfactory;

	m_effectManager = std::make_unique<PlayerEffectManager>();
	m_effectManager->Create();
	m_effectManager->Initialize(5.0f, Vector3::Zero);

	RECT outputSize = pDR->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	DirectX::SimpleMath::Matrix view = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(backBufferWidth) / float(backBufferHeight), 0.1f, 100.f);
	DirectX::SimpleMath::Matrix proj= Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	//������
	m_proveWorld = DirectX::SimpleMath::Matrix::Identity;

	//	�X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// �G�t�F�N�g�t�@�N�g���[
	m_fxFactory = std::make_unique<EffectFactory>(device);

	// ���\�[�X�f�B���N�g����ݒ肷��
	m_fxFactory->SetDirectory(L"Resources/Sdkmesh");

	// SDKMESH���f���𐶐�����
	m_model = Model::CreateFromSDKMESH(
		device,
		L"Resources/Sdkmesh/Walking.sdkmesh",
		*m_fxFactory,
		DirectX::ModelLoader_Clockwise | DirectX::ModelLoader_IncludeBones
	);

	// �A�j���[�V�����N���X�̃C���X�^���X�𐶐�����
	m_animation = std::make_unique<DX::AnimationSDKMESH>();

	// �A�j���[�V���������[�h����
	m_animation->Load(L"Resources/Sdkmesh/Walking.sdkmesh_anim");

	// ���f���ƃ{�[�����o�C���h����
	m_animation->Bind(*m_model);

	// �{�[���z��𐶐�����
	m_transformArray = DirectX::ModelBone::MakeArray(m_model->bones.size());

	// ���f���Ŏg�p���邷�ׂẴG�t�F�N�g���X�V
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		});

}

void Player::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);
	//�v���C���[�ɏd�͂�������
	m_position.y -= DROPSPEED;

	// �A�j���[�V�����̍X�V
	m_animation->Update(timer.GetElapsedSeconds());

	//�����_��2�ȉ��̒l�������Ō듮�삪�N���Ă���ׁA
	//�����^�ɂ��Ă��珬���_�ȉ���؂�̂�
	//
	m_position.x = round(m_position.x * 10) / 10;
	m_position.z = round(m_position.z * 10) / 10;
	
	//�v���C���[�̌��ݒn������o���ϐ�
	int indexX = static_cast<int>((m_position.x + 3.f + 1.25f) / 2.5f);
	int indexY = static_cast<int>((m_position.z + 1.25f) / 2.5f);
	
	//�v���C���[�����ꂩ�痎���Ă��Ȃ��Ƃ�
	if (m_fallFlag == false)
	{
		if (m_startFootHold == false)
		{
			//�Q�[���J�n���Ƀv���C���[������Ă��鑫�ꂪ�����Ă��Ȃ��Ƃ��A
			//�������͑��ꂪ�����Ă��邩�ǂ̃^�C��������ł��Ȃ��Ƃ�
			if (m_stageLoad->GetStageData(indexY, indexX) == 1 ||
				m_stageLoad->GetStageData(indexY, indexX) == 2 && m_footholdFlag == false)
			{
				m_position.y = 0.5f;
			}
		}
	}
	static const float fallLine = -2.9f;
	//�����艺�ɗ��������A�v���C���[�̑��삪�o���Ȃ��Ȃ�
	if (m_position.y < fallLine)
	{
		m_fallFlag = true;
	}

	//�J����������]������t���O
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::A) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraLeftFlag = true;
	}
	//�J�������E��]������t���O
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::D) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraRightFlag = true;
	}
	//�v���C���[�������Ă�ԁA�J�����͓������Ȃ��悤�ɂ���
	if (m_inputStopFlag == true)
	{
		m_cameraLeftFlag = false;
		m_cameraRightFlag = false;
	}
	//�J��������]
	if (m_cameraLeftFlag == true)
	{
		m_cameraLeftCount++;
		m_angle += MAX_ANGLE;
		if (m_cameraLeftCount >= MAX_ANGLE * 10)
		{
			m_cameraLeftCount = 0;
			m_cameraLeftFlag = false;
		}
	}
	//�J�����E��]
	if (m_cameraRightFlag == true)
	{
		m_cameraRightCount++;
		m_angle -= MAX_ANGLE;
		if (m_cameraRightCount >= MAX_ANGLE * 10)
		{
			m_cameraRightCount = 0;
			m_cameraRightFlag = false;
		}
	}
	//�l���傫���Ȃ肷�����菬�����Ȃ肷���Ȃ��悤�Ƀ��Z�b�g������
	if (m_angle == 360 || m_angle == -360)
	{
		m_angle = 0;
	}
	//=========================================================================

	//�v���C�V�[������Ď󂯎����bool�^�ϐ���true�ɂȂ邱�Ƃ�
	//�uMiss�v�̉摜���o���I���܂ŃJ�E���g�𑱂���ϐ��������o��
	if (m_resetPosition == true)
	{
		m_position = m_initialPosition;
	}
	//�����^�C����2�x���񂾎�
	if (m_velCountReset == true)
	{
		m_endDecCount++;
	}
	//240�ł͂Ȃ����R��stage�N���X��m_removeCount�̉e����
	//60�t���[���̃Y�����������邽��+60��300�ɂȂ��Ă���
	if (m_endDecCount >= 300)
	{
		//�v���C���[�̈ʒu��������
		m_position = m_initialPosition;
		m_endDecCount = 0;

		m_velocityCount = 25;
	}

	if (m_fallFlag == true)
	{
		m_velocityCount = 25;
	}
	if (m_velocityCount >= 25)
	{
		//�����I������̂Ńt���O��������
		m_inputStopFlag = false;
		//�v���C���[�̑S�Ă̍s���̃t���O��������
		m_upFlag = false;
		m_underFlag = false;
		m_rightFlag = false;
		m_leftFlag = false;
		m_velocityCount = 0;
	}

	m_effectManager->SetRenderState(m_pDebugCamera->GetEyePosition(), m_pDebugCamera->GetViewMatrix(), m_pDebugCamera->GetProjectionMatrix());

	m_effectManager->Update(timer);
}

void Player::Draw()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//	�ϊ��s��̐錾(���[���h�A�r���[�A�ˉe(�v���W�F�N�V����))
	DirectX::SimpleMath::Matrix world;
	
	//	���[���h�s��̏�����(�P�ʍs��)
	world = DirectX::SimpleMath::Matrix::Identity;
	//keyFlag���N���A����
	if (m_inputStopFlag == false)
	{
		ClearKeyFlag();
	}
	//�v���C���[��������ꍇ
	if (PlayerMove() == true)
	{
		//	�O
		if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_inputStopFlag==false)
		{
			m_upFlag = true;
			m_keyFlag[UP] = true;
		}
		//	��
		else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left) && m_inputStopFlag == false)
		{
			m_leftFlag = true;
			m_keyFlag[LEFT] = true;
		}
		//	��
		else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Down) && m_inputStopFlag == false)
		{
			m_underFlag = true;
			m_keyFlag[DOWN] = true;
		}
		//	�E
		else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right) && m_inputStopFlag == false)
		{
			m_rightFlag = true;
			m_keyFlag[RIGHT] = true;
		}
		//====================================================================================
		//��������L�[�������ꂽ���A
		//�J�����̌����ɉ����ăv���C���[���i�ތ�����ύX����
		//0�ȏ�̓J�������E��]�������Ƃ��̒l
		//0�ȉ��̓J����������]�������Ƃ��̒l
		float charaVel = 0.1f;

		if (m_keyFlag[UP] == true)
		{
			m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
			//�L�����������Ă�Œ��A���̗͂����������Ȃ�
			m_inputStopFlag = true;
			
			switch (m_angle)
			{
			case 0:
				m_position.z -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
				break;
			case 90:
			case -270:
				m_position.x -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
				break;
			case 180:
			case -180:
				m_position.z += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
				break;
			case 270:
			case -90:
				m_position.x += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(270));
				break;
			}
			m_velocityCount++;
			//3�̃L�����N�^�[���f����؂�ւ���^�C�}�[
			m_modelTimer++;
		}
		else if (m_keyFlag[DOWN] == true)
		{
			m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
			//�L�����������Ă�Œ��A���̗͂����������Ȃ�
			m_inputStopFlag = true;
			switch (m_angle)
			{
			case 0:
				m_position.z += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(180.0f * PI / 180.0f);
				break;
			case 90:
			case -270:
				m_position.x += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(270.0f * PI / 180.0f);
				break;
			case 180:
			case -180:
				m_position.z -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(0.0f * PI / 180.0f);
				break;
			case 270:
			case -90:
				m_position.x -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * PI);
				break;
			}
			m_velocityCount++;
			//3�̃L�����N�^�[���f����؂�ւ���^�C�}�[
			m_modelTimer++;
		}
		else if (m_keyFlag[RIGHT] == true)
		{
			m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
			//�L�����������Ă�Œ��A���̗͂����������Ȃ�
			m_inputStopFlag = true;
			switch (m_angle)
			{
			case 0:
				m_position.x += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(270.0f * PI / 180.0f);
				break;
			case 90:
			case -270:
				m_position.z -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(0.0f * PI / 180.0f);
				break;
			case 180:
			case -180:
				m_position.x -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * PI);
				break;
			case 270:
			case -90:
				m_position.z += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(180.0f * PI / 180.0f);
				break;
			}
			m_velocityCount++;
			//3�̃L�����N�^�[���f����؂�ւ���^�C�}�[
			m_modelTimer++;
		}
		else if (m_keyFlag[LEFT] == true)
		{
		m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
		//�L�����������Ă�Œ��A���̗͂����������Ȃ�
		m_inputStopFlag = true;
			switch (m_angle)
			{
			case 0:
				m_position.x -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * PI);
				break;
			case 90:
			case -270:
				m_position.z += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(180.0f * PI / 180.0f);
				break;
			case 180:
			case -180:
				m_position.x += charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(270.0f / 180.0f * PI);
				break;
			case 270:
			case -90:
				m_position.z -= charaVel;
				//��]
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(0.0f * PI / 180.0f);
				break;
			}
			m_velocityCount++;
			//3�̃L�����N�^�[���f����؂�ւ���^�C�}�[
			m_modelTimer++;
			
		}
		else
		{
			m_modelTimer = 0;
		}
	}
	// �g�k
	//world *= DirectX::SimpleMath::Matrix::CreateScale(0.5f, 0.5f, 0.5f);
	world *= DirectX::SimpleMath::Matrix::CreateScale(0.01f, 0.01f, 0.01f);
	//��]����ۑ������ϐ���world�ɑ��
	world *= m_proveWorld;
	// �ړ�
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	if (m_endDecCount >= 100)
	{
		//m_shader->Render();
	}
	
	auto hoge = world.Transpose();

	if (m_modelTimer >= 0 && m_modelTimer < 5)
	{
		// ���f���`��
		//m_pModel->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer >= 5 && m_modelTimer < 10)
	{
		//m_pModelLeft->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer >= 10 && m_modelTimer <= 15)
	{
		//m_pModel->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer >= 15 && m_modelTimer <= 20)
	{
		//m_pModelRight->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer == 20)
	{
		m_modelTimer = 0;
	}

	if (m_position.y < 0)
	{
		m_pModelFall->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	// �{�[���z��̃T�C�Y���擾����
	size_t nbones = m_model->bones.size();
	// �A�j���[�V�����Ƀ��f���A�{�[�����A�{�[����K�p����
	m_animation->Apply(*m_model, nbones, m_transformArray.get());

	//	�ϊ��s��̐錾(���[���h�A�r���[�A�ˉe(�v���W�F�N�V����))
	DirectX::SimpleMath::Matrix AnimationWorld;

	//	���[���h�s��̏�����(�P�ʍs��)
	AnimationWorld = DirectX::SimpleMath::Matrix::Identity;
	// �g�k
	AnimationWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.01f, 0.01f, 0.01f);

	// �A�j���[�V�����̕`��
	m_model->DrawSkinned(
		context,
		*m_commonState,
		nbones,
		m_transformArray.get(),
		world,
		m_playSceneCamera->GetView(), 
		m_playSceneCamera->GetProjection()
	);

}
//�v���C���[�������������
bool Player::PlayerMove()
{
	//1.�t�F�[�h�C���A�E�g���ł͂Ȃ��Ƃ�
	//2.�v���C���[�̈ʒu�����Z�b�g����Ă��Ȃ��Ƃ�
	//3.�v���C���[�������Ă��Ȃ�����ɂȂ��Ă���Ƃ�
	//4.�I���錾���n�܂���15�t���[���ȓ�(15�t���[���̗��R�́A�v���C���[���~�܂�܂ł̎���)�̂Ƃ�
	//��L�S�Ă̏����𖞂������Ƃ��v���C���[�͓������Ƃ��ł���
	if (m_textureFlag == true
		&& m_resetPosition == false && m_fallFlag == false
		&& m_endDecCount <= 15)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::ClearKeyFlag()
{
	//�ǂ̕����L�[�����͂ł����Ԃɂ���
	for (int i = 0; i < 4; i++)
	{
		m_keyFlag[i] = false;
	}
}