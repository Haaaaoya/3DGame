#include "pch.h"
//#include "DeviceResources.h"
//#include "EffectManager.h"
//#include <Effects.h> 
//#include <PrimitiveBatch.h> 
//using namespace DirectX;
//
//using namespace DirectX::SimpleMath;
//
//void EffectManager::Create()
//{
//	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
//
//	ID3D11Device1* device = pDR->GetD3DDevice();
//
//	//バッチエフェクトの作成
//	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
//	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
//	m_batchEffect->SetReferenceAlpha(255);
//	// 入力レイアウト生成
//	void const* shaderByteCode;
//	size_t byteCodeLength;
//	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
//	device->CreateInputLayout(
//		VertexPositionTexture::InputElements,
//		VertexPositionTexture::InputElementCount,
//		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
//	);
//
//	//プリミティブバッチの作成
//	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
//	
//	//テクスチャのロード
//	DirectX::CreateWICTextureFromFile(
//		device,
//		L"Resources/Textures/shadow.png",
//		nullptr,
//		m_texture.GetAddressOf()
//		);
//	
//
//	int count = 10;
//	for (int i = 0; i < count; i++) {
//		std::unique_ptr<ShaderSample> effect = std::make_unique<ShaderSample>();
//		effect->Create(/*m_texture.Get(), m_batchEffect.get(), m_batch.get(), m_inputLayout.Get());
//		m_effectList.push_back(std::move(effect)*/);
//		m_effectList.push_back(std::move(effect));
//	}
//
//		
//}
//
//void EffectManager::Initialize(float life,Vector3 pos)
//{
//	const float RAD = XM_PI * 2.0f;
//	int num = 0;
//
//
//	life = 20;
//	pos = Vector3::Zero;
//	
//	int range = 100;
//	//Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
//
//	//life,pos,vel の値でm_effectを初期化する
//	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
//		ite != m_effectList.end(); ite++)
//	{
//		float size = (float)m_effectList.size();
//		//Vector3 vel = Vector3(cos(RAD * num / size + XM_PI / 2.0f), sin(RAD * num / size + XM_PI / 2.0f),0.0f);
//		Vector3 vel = Vector3(0.0f, 2.0f, 0.0f);
//		vel *= 0.05f;
//		(*ite)->Initialize(life, pos, vel);
//		num++;
//	}
//	
//	/*while (vel.Length() < 0.03f)
//	{
//		vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
//	}*/
//}
//
//
//void EffectManager::Update(const DX::StepTimer& timer)
//{
//	
//	//timerを渡してm_effectの更新処理を行う
//	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
//		ite != m_effectList.end(); ite++)
//	{
//		(*ite)->Update(timer);
//	}
//
//}
//
//void EffectManager::Render()
//{
//	//m_effectを描画する
//	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
//		ite != m_effectList.end(); ite++)
//	{
//		(*ite)->Render();
//	}
//}
//
//void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
//{
//	//camera,view,proj,の値をm_effectに渡す
//	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_effectList.begin();
//		ite != m_effectList.end(); ite++)
//	{
//		(*ite)->SetRenderState(camera, view, proj);
//	}
//}
//
////void EffectManager::SetGravity(bool gravity)
////{
////	for (std::list<std::unique_ptr<MyEffect>>::iterator it = m_effectList.begin(); it != m_effectList.end(); ++it)
////	{
////		(*it)->SetGravity(gravity);
////	}
////}