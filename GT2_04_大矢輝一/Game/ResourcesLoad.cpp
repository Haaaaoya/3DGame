#include "pch.h"
#include "ResourcesLoad.h"

ResourcesLoad::ResourcesLoad()
	:
	TextureNameTable 
{
	L"Resources/Textures/rightBigDoor.png",
	L"Resources/Textures/rightShortDoor.png",
	L"Resources/Textures/leftBigDoor.png",
	L"Resources/Textures/leftShortDoor.png",
	L"Resources/Textures/ZKey.png",
	L"Resources/Textures/GameStart.png",
	L"Resources/Textures/StageCreate.png",
	L"Resources/Textures/Afewline.png",
	L"Resources/Textures/StageSelect.png",
	L"Resources/Textures/Arrow.png",
	L"Resources/Textures/frame.png",

	L"Resources/Textures/black.png",
	L"Resources/Textures/Stage.png",
	L"Resources/Textures/book.png",
	L"Resources/Textures/1Page.png",
	L"Resources/Textures/2Page.png",
	L"Resources/Textures/SpaceTexture.png",
	L"Resources/Textures/life.png",

	L"Resources/Textures/Clear.png",
	L"Resources/Textures/GameOver.png",
	L"Resources/Textures/Title.png",

	L"Resources/Textures/PaintingBase.png",
	L"Resources/Textures/Tile.png",
	L"Resources/Textures/showTexture.png",
	L"Resources/Textures/TransParentFloor.png",
	
	L"Resources/Textures/SaveTexture.png",
	L"Resources/Textures/Create1key.png",
	L"Resources/Textures/Create2key.png",
	L"Resources/Textures/Create3key.png",
}
{
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ResourcesLoad::GetTexture(ResourcesLoad::Texture texture)
{
	return m_textures[texture];
}

void ResourcesLoad::TitleTextureLoad()
{
	// D3Dデバイスとデバイスコンテキストの取得
	m_device = m_pDR->GetD3DDevice();

	for (int i = 0; i < 11; i++)
	{
		DirectX::CreateWICTextureFromFile(
			m_device,
			TextureNameTable[i],
			nullptr,
			m_textures[i].ReleaseAndGetAddressOf()
		);
	}
}

void ResourcesLoad::PlayTextureLoad()
{
	for (int i = 11; i < 18; i++)
	{
		DirectX::CreateWICTextureFromFile(
			m_device,
			TextureNameTable[i],
			nullptr,
			m_textures[i].ReleaseAndGetAddressOf()
		);
	}
}

void ResourcesLoad::ResultTextureLoad()
{
	for (int i = 18; i < 21; i++)
	{
		DirectX::CreateWICTextureFromFile(
			m_device,
			TextureNameTable[i],
			nullptr,
			m_textures[i].ReleaseAndGetAddressOf()
		);
	}
}

void ResourcesLoad::StageCreateTextureLoad()
{
	for (int i = 21; i < 29; i++)
	{
		DirectX::CreateWICTextureFromFile(
			m_device,
			TextureNameTable[i],
			nullptr,
			m_textures[i].ReleaseAndGetAddressOf()
		);
	}
}