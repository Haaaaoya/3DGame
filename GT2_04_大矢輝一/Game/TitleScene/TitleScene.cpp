/*
�^�C�g���V�[��
*/

#include "pch.h"

#include <chrono>
#include <thread>

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"
#include "TitleScene.h"
#include <Effects.h>
#include "Libraries/Adx2/Adx2.h"
#include <Helpers/CueSheet_0.h>

#define PI       (3.14f)

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
TitleScene::TitleScene() :
	m_rightBigMove(1400.0f, -100.0f),
	m_rightShortMove(1280.0f, -100.0f),
	m_leftBigMove(-700.0f, -100.0f),
	m_leftShortMove(-150.0f, -100.0f),
	m_changeTimer(0.0f),
	m_startFlag(false),
	m_framePosition(400, 500),
	m_stageCreateFlag(false),
	m_stageChoiceFlag(false),
	m_GameStartPosition{ 400,500 },
	m_StageCreatePosition{ 1000,500 },
	m_GameStartTimer(0.0f),
	m_StageCreateTimer(0.0f),
	m_arrowTexturePosition{300,150},
	m_alpha(1.0f),
	m_alphaFlag(true)
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
TitleScene::~TitleScene()
{
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void TitleScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
	//���摜�̈ʒu�̏�����
	m_rightBigPosition = m_rightBigMove;
	m_rightShortPosition = m_rightShortMove;
	m_leftBigPosition = m_leftBigMove;
	m_leftShortPosition = m_leftShortMove;
	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	//�X�e�[�W�̏����l
	m_stageNumber = 1;

	ADX2::GetInstance().Initialize("Resources/Sounds/song.acf", "Resources/Sounds/CueSheet_0.acb");
	ADX2::GetInstance().Play(CRI_CUESHEET_0_�^�C�g��);

	// �S�̂̉��ʂ̒����ivolume��0.0f�`1.0f�j
	ADX2::GetInstance().SetVolume(0.4f);

	ResourcesLoad* resourcesLoad;
	resourcesLoad = ResourcesLoad::GetInstance();
	resourcesLoad->TitleTextureLoad();
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);
	// �}�E�X���͏����擾����
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	//���̉摜�̈ʒu�̌Œ�l
	static const int RightBigDoorPosition = 610;
	static const int RightShortDoorPosition = 1150;
	static const int LeftBigDoorPosition = 90;
	static const int LeftShortDoorPosition = -30;
	//���̍X�V
	ADX2::GetInstance().Update();

	//�Q�[���X�^�[�g
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) && m_stageChoiceFlag == true)
	{
		m_startFlag = true;
		ADX2::GetInstance().Play(CRI_CUESHEET_0_����{�^��������52);
	}

	//�X�e�[�W�I����ʂɈړ�
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) && m_stageCreateFlag == false && m_stageChoiceFlag == false)
	{
		m_stageChoiceFlag = true;
		ADX2::GetInstance().Play(CRI_CUESHEET_0_����{�^��������52);
	}
	
	//Quit��I�𒆂�Z�L�[�������ƃQ�[���I��
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Z) && m_stageCreateFlag == true && m_stageChoiceFlag == false)
	{
		return GAME_SCENE::STAGECREATE;
	}

	//���摜�𓮂����ϐ��ƃQ�[���J�n���邽�߂̃J�E���g�ϐ�
	if (m_startFlag == true)
	{
		m_changeTimer += 1 / 60.0f;

		m_leftShortMove.x += 10.0f;
		m_leftBigMove.x += 20.0f;
		m_rightBigMove.x -= 20.0f;
		m_rightShortMove.x -= 10.0f;
	}
	if (m_changeTimer >= 3.0f)
	{
		return GAME_SCENE::PLAY;
	}
	//�摜�̈ʒu�̏���A�����l�����߂�
	//=============================================================
	if (m_leftBigMove.x > LeftBigDoorPosition)
	{
		m_leftBigMove.x = LeftBigDoorPosition;
	}

	if (m_leftShortMove.x > LeftShortDoorPosition)
	{
		m_leftShortMove.x = LeftShortDoorPosition;
	}

	if (m_rightBigMove.x < RightBigDoorPosition)
	{
		m_rightBigMove.x = RightBigDoorPosition;
	}

	if (m_rightShortMove.x < RightShortDoorPosition)
	{
		m_rightShortMove.x = RightShortDoorPosition;
	}
	//=============================================================
	m_rightBigPosition = m_rightBigMove;
	m_rightShortPosition = m_rightShortMove;
	m_leftBigPosition = m_leftBigMove;
	m_leftShortPosition = m_leftShortMove;

	//�t���[����StageCreate�������Ă��鎞���E�L�[�������ꂽ��
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right) && m_stageCreateFlag == false && m_stageChoiceFlag == false)
	{
		m_stageCreateFlag = true;
		ADX2::GetInstance().Play(CRI_CUESHEET_0_����{�^��������52);
	}
	//�t���[����GameStart�������Ă��鎞�����L�[�������ꂽ��
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left) && m_stageCreateFlag == true && m_stageChoiceFlag == false)
	{
		m_stageCreateFlag = false;
		ADX2::GetInstance().Play(CRI_CUESHEET_0_����{�^��������52);
	}

	float time = timer.GetElapsedSeconds();
	static const float MOVETIME = 0.4f;
	//�I���摜�𓮂�������
	//������E�ֈړ�
	if (m_stageCreateFlag == true)
	{
		m_StageCreateTimer += time;
		m_GameStartTimer = 0.0f;
		m_framePosition = DirectX::SimpleMath::Vector2::Lerp(m_framePosition, m_StageCreatePosition, m_StageCreateTimer / MOVETIME);
	}
	//�E���獶�ֈړ�
	if (m_stageCreateFlag == false)
	{
		m_GameStartTimer += time;
		m_StageCreateTimer = 0.0f;
		m_framePosition = DirectX::SimpleMath::Vector2::Lerp(m_framePosition, m_GameStartPosition, m_GameStartTimer / MOVETIME);
	}
	//�t���[���摜��������͈͂𐧌����Ă���
	if (m_framePosition.x < m_GameStartPosition.x)
	{
		m_framePosition = m_GameStartPosition;
	}
	else if (m_framePosition.x > m_StageCreatePosition.x)
	{
		m_framePosition = m_StageCreatePosition;
	}
	//�X�e�[�W��I�������ʂ̏�Ԃ̎�
	if (m_stageChoiceFlag == true)
	{
		if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up))
		{
			m_stageNumber--;
		}

		if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Down))
		{
			m_stageNumber++;
		}
	}
	int minStageNumber = 1;
	int maxStageNumber = 6;
	//Clamp
	m_stageNumber = std::min(std::max(m_stageNumber, minStageNumber), maxStageNumber);

	//���摜�̈ʒu
	m_arrowTexturePosition = DirectX::SimpleMath::Vector2{ static_cast<float>(300),static_cast <float>(40 + 100 * m_stageNumber) };
	if (m_stageNumber == maxStageNumber)
	{
		m_arrowTexturePosition.x = 150;
	}

	//�A���t�@�l�̕ϓ�
	if (m_alphaFlag == true)
	{
		m_alpha += 0.04f;
	}
	else
	{
		m_alpha -= 0.04f;
	}

	//�A���t�@�l�̐���
	if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
		m_alphaFlag = false;
	}
	if (m_alpha < 0.0f)
	{
		m_alpha = 0.0f;
		m_alphaFlag = true;
	}
	
	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void TitleScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());
	//Z�L�[�̈ʒu
	DirectX::SimpleMath::Vector2 keyPosition(400, 650);
	if (m_stageChoiceFlag == false)
	{
		//Z�L�[�����Ă��������摜
		m_spriteBatch->Draw(
			ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::ZKey).Get(),
			keyPosition,
			DirectX::FXMVECTOR{ 1.0f,1.0f,1.0f,m_alpha }
		);
		
		//�Q�[���X�^�[�g�摜
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::GameStart).Get(), m_GameStartPosition,
			0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(200, 100));
		//StageCreate�摜
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::StageCreateTexture).Get(), m_StageCreatePosition,
			0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(200, 100));
		//�t���[���摜
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Frame).Get(), m_framePosition,
			0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(225, 90));

		//�^�C�g���摜�\��
		DirectX::SimpleMath::Vector2 titlePos = DirectX::SimpleMath::Vector2(250, 120);
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Afewline).Get(), titlePos, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 2.0f);
	}
	else
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::StageSelect).Get(), DirectX::SimpleMath::Vector2(800, 400),
			0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(450, 540));
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Arrow).Get(), m_arrowTexturePosition,
			0, { 1.0f,1.0f,1.0f,m_alpha }, 0.0f, DirectX::SimpleMath::Vector2(175, 255),0.7f);
		//Z�L�[�����Ă��������摜
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::ZKey).Get(), keyPosition, { 1.0f,1.0f,1.0f,m_alpha });
	}

	//���摜
	//=======================================================================================================
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::RightBigDoor).Get(), m_rightBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::RightShortDoor).Get(), m_rightShortPosition, 0, DirectX::Colors::White);

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::LeftBigDoor).Get(), m_leftBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::LeftShortDoor).Get(), m_leftShortPosition, 0, DirectX::Colors::White);
	//=======================================================================================================
	m_spriteBatch->End();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void TitleScene::Finalize()
{
	ADX2::GetInstance().Finalize();
}

bool TitleScene::GetClearFlag() 
{
	return 0;
}

int TitleScene::GetStageNum()
{
	//�ǂ̃X�e�[�W���I�΂ꂽ�̂��`������
	return m_stageNumber;
}

/*--------------------------------------------------
���\�[�X�̓ǂݍ���
--------------------------------------------------*/
void TitleScene::LoadResources()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
}