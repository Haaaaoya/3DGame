//2022,05,31
//制作者：加藤慎也
//モデルのデータの管理、描画を行うクラス
#include"pch.h"

#include"ModelResources.h"

#include"DeviceResources.h"


//モデルのデータをロードしモデルクラスを作成、連想配列に格納する関数
//引数：登録したいモデルのタグ名,読み込むモデルのファイル名(パス)
//戻り値：無し
void ModelResources::LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName)
{
	DX::DeviceResources* device = DX::DeviceResources::GetInstance();
	ID3D11Device1* device2 = device->GetD3DDevice();

	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> fxFactory;
	fxFactory = std::make_unique<DirectX::EffectFactory>(device2);

	m_models[modelTag] = std::make_unique<DirectX::Model>();

	fxFactory->SetDirectory(L"Resources/Models");
	m_models[modelTag] = DirectX::Model::CreateFromCMO(device2, modelFileName, *fxFactory);
}

//モデルのデータをロードしモデルクラスを作成、連想配列に格納する関数
//引数：
//登録したいモデルのタグ名
//読み込むモデルのファイル名(パス)
//エミッションさせたい色
//戻り値：無し
void ModelResources::LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName,const DirectX::SimpleMath::Vector3& emissionColor)
{
	DX::DeviceResources* device = DX::DeviceResources::GetInstance();
	ID3D11Device1* device2 = device->GetD3DDevice();

	m_models[modelTag] = std::make_unique<DirectX::Model>();

	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> fxFactory;
	fxFactory = std::make_unique<DirectX::EffectFactory>(device2);

	fxFactory->SetDirectory(L"Resources/Models");
	m_models[modelTag] = DirectX::Model::CreateFromCMO(device2, modelFileName, *fxFactory);

	//エミッション設定(自己発光)
	//ライトの影響を受けた部分が指定した色の上限値に設定される
	m_models[modelTag]->UpdateEffects([&](DirectX::IEffect* effect)
		{
			DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			if (lights)
			{
				// ライトの影響をなくす
				lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			if (basicEffect)
			{
				// エミッション色を白に設定する(自己発光)
				basicEffect->SetEmissiveColor(emissionColor);
			}
		});

}