//2022,05,31
//����ҁF�����T��
//���f���̃f�[�^�̊Ǘ��A�`����s���N���X
#include"pch.h"

#include"ModelResources.h"

#include"DeviceResources.h"


//���f���̃f�[�^�����[�h�����f���N���X���쐬�A�A�z�z��Ɋi�[����֐�
//�����F�o�^���������f���̃^�O��,�ǂݍ��ރ��f���̃t�@�C����(�p�X)
//�߂�l�F����
void ModelResources::LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName)
{
	DX::DeviceResources* device = DX::DeviceResources::GetInstance();
	ID3D11Device1* device2 = device->GetD3DDevice();

	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> fxFactory;
	fxFactory = std::make_unique<DirectX::EffectFactory>(device2);

	m_models[modelTag] = std::make_unique<DirectX::Model>();

	fxFactory->SetDirectory(L"Resources/Models");
	m_models[modelTag] = DirectX::Model::CreateFromCMO(device2, modelFileName, *fxFactory);
}

//���f���̃f�[�^�����[�h�����f���N���X���쐬�A�A�z�z��Ɋi�[����֐�
//�����F
//�o�^���������f���̃^�O��
//�ǂݍ��ރ��f���̃t�@�C����(�p�X)
//�G�~�b�V�������������F
//�߂�l�F����
void ModelResources::LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName,const DirectX::SimpleMath::Vector3& emissionColor)
{
	DX::DeviceResources* device = DX::DeviceResources::GetInstance();
	ID3D11Device1* device2 = device->GetD3DDevice();

	m_models[modelTag] = std::make_unique<DirectX::Model>();

	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> fxFactory;
	fxFactory = std::make_unique<DirectX::EffectFactory>(device2);

	fxFactory->SetDirectory(L"Resources/Models");
	m_models[modelTag] = DirectX::Model::CreateFromCMO(device2, modelFileName, *fxFactory);

	//�G�~�b�V�����ݒ�(���Ȕ���)
	//���C�g�̉e�����󂯂��������w�肵���F�̏���l�ɐݒ肳���
	m_models[modelTag]->UpdateEffects([&](DirectX::IEffect* effect)
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
				// �G�~�b�V�����F�𔒂ɐݒ肷��(���Ȕ���)
				basicEffect->SetEmissiveColor(emissionColor);
			}
		});

}