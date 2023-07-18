#pragma once
#include <unordered_map>
#include <WICTextureLoader.h>
#include <DeviceResources.h>

class ResourcesLoad
{
public:
	enum Texture
	{
		RightBigDoor,
		RightShortDoor,
		LeftBigDoor,
		LeftShortDoor,
		ZKey,
		GameStart,
		StageCreateTexture,
		Afewline,
		StageSelect,
		Arrow,
		Frame,

		TextureBlack,
		TextureStage,
		TextureManual,
		TextureManual1Page,
		TextureManual2Page,
		TextureSpacekey,
		TextureLife,

		Clear,
		GameOver,
		Title,

		PaintingBase,
		Tile,
		showTexture,
		TransParentFloor,
		SaveTexture,
		Create1key,
		Create2key,
		Create3key,

		TextureNum
	};

	static ResourcesLoad* GetInstance()
	{
		static ResourcesLoad resourcesLoad;
		return &resourcesLoad;
	}

	ResourcesLoad(const ResourcesLoad&) = delete;
	ResourcesLoad& operator=(const ResourcesLoad&) = delete;
	ResourcesLoad(const ResourcesLoad&&) = delete;
	ResourcesLoad& operator=(const ResourcesLoad&&) = delete;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(ResourcesLoad::Texture texture);

	void TitleTextureLoad();
	void PlayTextureLoad();
	void ResultTextureLoad();
	void StageCreateTextureLoad();
private:
	ResourcesLoad();
	~ResourcesLoad() = default;

	// テクスチャ名テーブル
	wchar_t* TextureNameTable[ResourcesLoad::Texture::TextureNum];
	std::unordered_map<Texture, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textureID;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textures[TextureNum];


	DX::DeviceResources* m_pDR = DX::DeviceResources::GetInstance();
	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* m_device;
};