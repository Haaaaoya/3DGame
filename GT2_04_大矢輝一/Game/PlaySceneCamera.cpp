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

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	m_view = DirectX::SimpleMath::Matrix::
		CreateLookAt(
			m_cameraPosition,
			m_moveTarget,
			DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f)
		);

	//	プロジェクション
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);		//	画角
	float aspectRatio = width / height;								//	画面縦横比
	float nearPlane = 1.0f;											//	カメラから一番近い投影面
	float farPlane = 1000000.0f;									//	カメラから一番遠い投影面
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PlaySceneCamera::FixedCameraSetting()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	m_view = DirectX::SimpleMath::Matrix::
		CreateLookAt(
			m_cameraPosition,
			m_fixedTarget,
			DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f)
		);

	//	プロジェクション
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);		//	画角
	float aspectRatio = width / height;								//	画面縦横比
	float nearPlane = 1.0f;											//	カメラから一番近い投影面
	float farPlane = 1000000.0f;									//	カメラから一番遠い投影面
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}