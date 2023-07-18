/*
���[�h���
*/
#pragma once

#include <thread>
#include <mutex>

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include <SimpleMath.h>

#include "../ILoadingScreen.h"
#include <vector>


class LoadingScreen : public ILoadingScreen
{
private:

	// ���\�[�X�ǂݍ��ݗp
	std::thread m_LoadingThread;
	bool m_isLoading;
	std::mutex m_LoadingMutex;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// �E�̑傫�Ȕ��e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rightBigDoor;

	// �E�̏����Ȕ��e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rightShortDoor;

	// ���̑傫�Ȕ��e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_leftBigDoor;

	// ���̏����Ȕ��e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_leftShortDoor;

	
	float count;
	//�L�����𓮂����J�E���g
	float m_moveCount;
	//�w�i�̐؂�ւ��J�E���g
	float m_textureCount;

	DirectX::SimpleMath::Vector2 m_rightBigPosition;
	DirectX::SimpleMath::Vector2 m_rightShortPosition;
	DirectX::SimpleMath::Vector2 m_leftBigPosition;
	DirectX::SimpleMath::Vector2 m_leftShortPosition;

	//���ꂼ��̔��̈ʒu�̌Œ�l
	float m_rightBigMove;
	float m_rightShortMove;
	float m_leftBigMove;
	float m_leftShortMove;

	//���Y���W
	std::vector<float> posY;
	

	RECT m_charaRECT;

public:

	// �R���X�g���N�^
	LoadingScreen();

	// �f�X�g���N�^
	~LoadingScreen();

	// ������
	void Initialize(IScene* targetScene) override;

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// �ǂݍ��ݒ����ǂ����̎擾
	bool IsLoading();
private:
	// �ΏۃV�[���̃��\�[�X�̓ǂݍ���
	void LoadSceneResources(IScene* targetScene) override;

};