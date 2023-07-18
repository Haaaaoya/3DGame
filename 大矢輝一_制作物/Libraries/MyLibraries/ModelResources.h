//2022,05,31
//����ҁF�����T��
//���f���̃f�[�^�̊Ǘ��A�`����s���N���X
#pragma once

//�G�t�F�N�g
#include <Effects.h>
//���f��
#include <Model.h>
//�V���v���}�X
#include<SimpleMath.h>

//�}�b�v
#include <unordered_map>



class ModelResources
{
public:
	//�C���X�^���X�̑���
	static ModelResources* GetInstance()
	{
		static ModelResources instance;
		return &instance;
	}
private:
	//���f��
	std::unordered_map<wchar_t*, std::unique_ptr<DirectX::Model>> m_models;

private:
	//�R���X�g���N�^��f�X�g���N�^�͊O������A�N�Z�X�ł��Ȃ��悤��
	ModelResources() = default;
	~ModelResources() = default;

	//�R�s�[�R���X�g���N�^�A�R�s�[������Z�q�̍폜
	ModelResources(const ModelResources&) = delete;
	ModelResources& operator=(const ModelResources&) = delete;

	//���[�u�R���X�g���N�^�A������[�u���Z�q�̍폜
	ModelResources(const ModelResources&&) = delete;
	ModelResources&& operator=(const ModelResources&&) = delete;

public:
	//���f���̃f�[�^�����[�h�����f���N���X���쐬�A�A�z�z��Ɋi�[����֐�
	//�����F
	//�o�^���������f���̃^�O��
	//�ǂݍ��ރ��f���̃t�@�C����(�p�X)
	//�߂�l�F����
	void LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName);

	//���f���̃f�[�^�����[�h�����f���N���X���쐬�A�A�z�z��Ɋi�[����֐�
	//�����F
	//�o�^���������f���̃^�O��
	//�ǂݍ��ރ��f���̃t�@�C����(�p�X)
	//�G�~�b�V�������������F
	//�߂�l�F����
	void LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName,const DirectX::SimpleMath::Vector3& emissionColor);


	//�^�O�Ŏw�肳�ꂽ���f���̎擾
	//�����F�擾���������f���̃^�O
	//�߂�l�F���f��
	DirectX::Model* GetModelData(wchar_t* modelTag) { return m_models.at(modelTag).get(); };

};