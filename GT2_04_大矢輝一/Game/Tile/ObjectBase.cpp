#include "pch.h"
#include "ObjectBase.h"
#include "DeviceResources.h"
#include <WICTextureLoader.h>

ObjectBase::ObjectBase()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
}

ObjectBase::~ObjectBase()
{}

void ObjectBase::Initialize()
{
}

void ObjectBase::Update()
{
	m_position = m_target;
}

void ObjectBase::Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{

}