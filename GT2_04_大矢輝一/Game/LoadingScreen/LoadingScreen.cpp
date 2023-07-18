/*
���[�h���
*/

#include "pch.h"
#include <WICTextureLoader.h>
#include "LoadingScreen.h"
#include <SimpleMath.h>


#include "DeviceResources.h"
#include "../IScene.h"
#include <SimpleMath.h>

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
LoadingScreen::LoadingScreen()
	:
	m_isLoading{false},
	count(0.0f),
	m_charaRECT{0,224,32,256},
	m_moveCount(0.0f),
	m_textureCount(0.0f),
	posY(3,-100),
	m_rightBigMove(610.0f),
	m_rightShortMove(1150.0f),
	m_leftBigMove(90.0f),
	m_leftShortMove(-30.0f)
{
	for (int i = 0; i < 3; i++)
	{
		posY[i] = posY[i] * (i * 2);
	}
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
LoadingScreen::~LoadingScreen()
{
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void LoadingScreen::Initialize(IScene* targetScene)
{
	// �ΏۃV�[���̃��\�[�X�̓ǂݍ���
	LoadSceneResources(targetScene);


	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");


	// �e�N�X�`���̓ǂݍ���
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/NowLoading.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/rightBigDoor.png",
		nullptr,
		m_rightBigDoor.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/rightShortDoor.png",
		nullptr,
		m_rightShortDoor.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/leftBigDoor.png",
		nullptr,
		m_leftBigDoor.ReleaseAndGetAddressOf()
	);

	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/leftShortDoor.png",
		nullptr,
		m_leftShortDoor.ReleaseAndGetAddressOf()
	);

	m_rightBigPosition = DirectX::SimpleMath::Vector2(m_rightBigMove, -100.0f);
	m_rightShortPosition = DirectX::SimpleMath::Vector2(m_rightShortMove, -100.0f);
	m_leftBigPosition = DirectX::SimpleMath::Vector2(m_leftBigMove, -100.0f);
	m_leftShortPosition = DirectX::SimpleMath::Vector2(m_leftShortMove, -100.0f);
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
void LoadingScreen::Update(const DX::StepTimer& timer)
{
	
	


	//�L�����̉摜��؂蔲��
	if (m_moveCount >= 0.0f && m_moveCount < 40.0f)
	{
		m_charaRECT = { 64,224,96,256 };
	}
	if (m_moveCount >= 40.0f && m_moveCount < 80.0f)
	{
		m_charaRECT = { 32,224,64,256 };
	}
	if (m_moveCount >= 80.0f && m_moveCount < 120.0f)
	{
		m_charaRECT = { 0,224,32,256 };
	}

	if (m_moveCount > 120.0f)
	{
		m_moveCount = 0.0f;
	}
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void LoadingScreen::Draw()
{
	
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	
	m_spriteBatch->Draw(m_rightBigDoor.Get(), m_rightBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(m_rightShortDoor.Get(), m_rightShortPosition, 0, DirectX::Colors::White);

	m_spriteBatch->Draw(m_leftBigDoor.Get(), m_leftBigPosition, 0, DirectX::Colors::White, 0, DirectX::g_XMZero, 1.1f);

	m_spriteBatch->Draw(m_leftShortDoor.Get(), m_leftShortPosition, 0, DirectX::Colors::White);

	m_spriteBatch->Draw(m_texture.Get(), DirectX::SimpleMath::Vector2(850, 650),
		0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(300, 35), 1.0f);

	m_spriteBatch->End();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void LoadingScreen::Finalize()
{
}

/*--------------------------------------------------
	// �ǂݍ��ݒ����ǂ����̎擾
--------------------------------------------------*/
bool LoadingScreen::IsLoading()
{
	m_LoadingMutex.lock();
	bool isLoading = m_isLoading;
	m_LoadingMutex.unlock();

	return isLoading;
}

/*--------------------------------------------------
�V�[���̃��\�[�X�̓ǂݍ���
--------------------------------------------------*/
void LoadingScreen::LoadSceneResources(IScene* targetScene)
{
	if (m_LoadingThread.joinable())
	{
		m_LoadingThread.join();
	}

	m_isLoading = true;
	m_LoadingThread = std::thread(
		[=]
		{
			targetScene->LoadResources();

			m_LoadingMutex.lock();
			m_isLoading = false;
			m_LoadingMutex.unlock();
		});
}
