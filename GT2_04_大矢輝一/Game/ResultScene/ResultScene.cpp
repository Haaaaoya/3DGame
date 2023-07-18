/*
���U���g�V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"
#include "Game/ResultScene/ResultScene.h"
#include "Libraries/Adx2/Adx2.h"
#include <Helpers/CueSheet_0.h>

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
ResultScene::ResultScene(bool clear):
	m_rightBigMove(1200.0f, -100.0f),
	m_rightShortMove(1280.0f, -100.0f),
	m_leftBigMove(-500.0f, -100.0f),
	m_leftShortMove(-150.0f, -100.0f),
	m_textureCount(0),
	m_alpha(1.0f),
	m_scaleValue(0.6f)
{
	m_clear = clear;
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
ResultScene::~ResultScene()
{
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void ResultScene::Initialize()
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

	ADX2::GetInstance().Initialize("Resources/Sounds/song.acf", "Resources/Sounds/CueSheet_0.acb");
	// �S�̂̉��ʂ̒����ivolume��0.0f�`1.0f�j
	ADX2::GetInstance().SetVolume(0.4f);

	if (m_clear == false)
	{
		m_alpha = 0.0f;
		ADX2::GetInstance().Play(CRI_CUESHEET_0_�Q�[���I�[�o�[);
	}
	else
	{
		ADX2::GetInstance().Play(CRI_CUESHEET_0_�Q�[���N���A�[�I);
	}
	//SE�̏�����
	for (int i = 0; i < 5; i++)
	{
		m_seFlag[i] = false;
	}

	ResourcesLoad::GetInstance()->ResultTextureLoad();
}

GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// �}�E�X���͏����擾����
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();
	//���̍X�V
	ADX2::GetInstance().Update();
	//���̉摜�̈ړ��ʂ̌Œ�l
	static const int BigDoorVelocity = 40;
	static const int ShortDoorVelocity = 20;
	//���̉摜�̈ʒu�̌Œ�l
	static const int RightBigDoorPosition = 610;
	static const int RightShortDoorPosition = 1150;
	static const int LeftBigDoorPosition = 90;
	static const int LeftShortDoorPosition = -30;

	//�N���A���ĂȂ��ꍇ
	if (m_clear == false && m_textureCount > 100
		&& m_textureCount < 120)
	{
		m_alpha += 0.05f;
	}

	if (m_textureCount < 60)
	{
		m_leftShortMove.x += ShortDoorVelocity;
		m_leftBigMove.x += BigDoorVelocity;
		m_rightBigMove.x -= BigDoorVelocity;
		m_rightShortMove.x -= ShortDoorVelocity;
	}
	if (m_textureCount > 300 && m_textureCount < 360)
	{
		m_leftShortMove.x -= ShortDoorVelocity;
		m_leftBigMove.x -= BigDoorVelocity;
		m_rightBigMove.x += BigDoorVelocity;
		m_rightShortMove.x += ShortDoorVelocity;
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
	//�摜�̈ʒu�����肷��
	m_rightBigPosition = m_rightBigMove;
	m_rightShortPosition = m_rightShortMove;
	
	m_leftBigPosition= m_leftBigMove;
	m_leftShortPosition = m_leftShortMove;
	//�J�E���g�̍X�V
	m_textureCount++;
	//�uCLEAR�v��alpha�l��������
	if (m_textureCount > 250)
	{
		m_alpha -= 0.05f;
	}

	if (m_textureCount > 450)
	{
		return GAME_SCENE::TITLE;
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void ResultScene::Draw()
{
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//
	std::vector<DirectX::SimpleMath::Vector2> clearPos(5);
	DirectX::SimpleMath::Vector2 blackPos(0, 0);
	DirectX::SimpleMath::Vector2 gameOverPos(100, 150);

	//�^�C�g���̉摜��\������
	if (m_textureCount > 120)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Title).Get(), DirectX::SimpleMath::Vector2(-5, 0),0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.005f);
	}

	//���摜
	//=======================================================================================================
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::RightBigDoor).Get(), m_rightBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::RightShortDoor).Get(), m_rightShortPosition, 0, DirectX::Colors::White);

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::LeftBigDoor).Get(), m_leftBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::LeftShortDoor).Get(), m_leftShortPosition, 0, DirectX::Colors::White);
	//=======================================================================================================
	//�N���A�����ꍇ
	if (m_clear == true)
	{
		for (int i = 0; i < 5; i++)
		{
			clearPos[i] = DirectX::SimpleMath::Vector2(350 + i * 150, 300);
			m_rect[i] = { 85 * i, 0 ,85 * (i + 1), 110 };
		}
		if (m_textureCount > 60)
		{
			//�摜�̑傫�������������Ă���
			m_scale[0] -= m_scaleValue;
			if (m_scale[0] < 1.6f && m_seFlag[0] == false)
			{
				m_seFlag[0] = true;
				ADX2::GetInstance().Play(CRI_CUESHEET_0_�葫�E�󂯎~�߂�02);
			}
			if (m_seFlag[0] == true)
			{
				m_scale[0] = 1.5f;
			}
			//�uC�v��`��
			m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Clear).Get(), clearPos[0], &m_rect[0], DirectX::SimpleMath::Vector4(1, 1, 1, m_alpha),
				0.0f, DirectX::SimpleMath::Vector2(42, 55), m_scale[0]);
		}
		if (m_textureCount > 70)
		{
			//�摜�̑傫�������������Ă���
			m_scale[1] -= m_scaleValue;
			if (m_scale[1] < 1.6f && m_seFlag[1] == false)
			{
				m_seFlag[1] = true;
				ADX2::GetInstance().Play(CRI_CUESHEET_0_�葫�E�󂯎~�߂�02);
			}
			if (m_seFlag[1] == true)
			{
				m_scale[1] = 1.5f;
			}
			//�uL�v��`��
			m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Clear).Get(), clearPos[1], &m_rect[1], DirectX::SimpleMath::Vector4(1, 1, 1, m_alpha),
				0.0f, DirectX::SimpleMath::Vector2(42, 55), m_scale[1]);
		}
		if (m_textureCount > 80)
		{
			//�摜�̑傫�������������Ă���
			m_scale[2] -= m_scaleValue;
			if (m_scale[2] < 1.6f && m_seFlag[2] == false)
			{
				m_seFlag[2] = true;
				ADX2::GetInstance().Play(CRI_CUESHEET_0_�葫�E�󂯎~�߂�02);
			}
			if (m_seFlag[2] == true)
			{
				m_scale[2] = 1.5f;
			}
			//�uE�v��`��
			m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Clear).Get(), clearPos[2], &m_rect[2], DirectX::SimpleMath::Vector4(1, 1, 1, m_alpha),
				0.0f, DirectX::SimpleMath::Vector2(42, 55), m_scale[2]);
		}
		if (m_textureCount > 90)
		{
			//�摜�̑傫�������������Ă���
			m_scale[3] -= m_scaleValue;
			if (m_scale[3] < 1.6f && m_seFlag[3] == false)
			{
				m_seFlag[3] = true;
				ADX2::GetInstance().Play(CRI_CUESHEET_0_�葫�E�󂯎~�߂�02);
			}
			if (m_seFlag[3] == true)
			{
				m_scale[3] = 1.5f;
			}
			//�uA�v��`��
			m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Clear).Get(), clearPos[3], &m_rect[3], DirectX::SimpleMath::Vector4(1, 1, 1, m_alpha),
				0.0f, DirectX::SimpleMath::Vector2(42, 55), m_scale[3]);
		}
		if (m_textureCount > 100)
		{
			//�摜�̑傫�������������Ă���
			m_scale[4] -= 0.6f;
			if (m_scale[4] < 1.6f && m_seFlag[4] == false)
			{
				m_seFlag[4] = true;
				ADX2::GetInstance().Play(CRI_CUESHEET_0_�葫�E�󂯎~�߂�02);
			}
			if (m_seFlag[4] == true)
			{
				m_scale[4] = 1.5f;
			}
			//�uR�v��`��
			m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Clear).Get(), clearPos[4], &m_rect[4], DirectX::SimpleMath::Vector4(1, 1, 1, m_alpha), 0.0f, DirectX::SimpleMath::Vector2(42, 55), m_scale[4]);
		}
	}
	//�N���A���ĂȂ��ꍇ
	else
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::GameOver).Get(), gameOverPos, 0, DirectX::SimpleMath::Vector4(1, 1, 1, m_alpha));
	}
	m_spriteBatch->End();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void ResultScene::Finalize()
{
	ADX2::GetInstance().Finalize();
}

bool ResultScene::GetClearFlag() 
{
	return 0;
}

int ResultScene::GetStageNum()
{
	return 0;
}

/*--------------------------------------------------
���\�[�X�̓ǂݍ���
--------------------------------------------------*/
void ResultScene::LoadResources()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
}