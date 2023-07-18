#include "pch.h"
#include "PlaySceneCamera.h"
#include "DeviceResources.h"

PlaySceneCamera::PlaySceneCamera()
{

}

PlaySceneCamera::~PlaySceneCamera()
{

}

void PlaySceneCamera::MoveCameraSetting()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	m_view = DirectX::SimpleMath::Matrix::
		CreateLookAt(
			m_cameraPosition,
			m_moveTarget,
			DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f)
		);

	//	�v���W�F�N�V����
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);		//	��p
	float aspectRatio = width / height;								//	��ʏc����
	float nearPlane = 1.0f;											//	�J���������ԋ߂����e��
	float farPlane = 1000000.0f;									//	�J���������ԉ������e��
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PlaySceneCamera::FixedCameraSetting()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	m_view = DirectX::SimpleMath::Matrix::
		CreateLookAt(
			m_cameraPosition,
			m_fixedTarget,
			DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f)
		);

	//	�v���W�F�N�V����
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);		//	��p
	float aspectRatio = width / height;								//	��ʏc����
	float nearPlane = 1.0f;											//	�J���������ԋ߂����e��
	float farPlane = 1000000.0f;									//	�J���������ԉ������e��
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}