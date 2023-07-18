#include "pch.h"
#include "P_ShaderSample.h"
#include "BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

const std::vector<D3D11_INPUT_ELEMENT_DESC> PlayerShaderSample::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

PlayerShaderSample::PlayerShaderSample()
{
}

PlayerShaderSample::~PlayerShaderSample()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	シェーダ等を生成
//	IN	:	デバイス			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void PlayerShaderSample::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto device = pDR->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	const wchar_t* name = L"Resources\\Textures\\shadow2.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());

	const wchar_t* name2 = L"Resources\\Textures\\ball.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name2, nullptr, m_texture2.GetAddressOf());

	// プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage		=	D3D11_USAGE_DEFAULT;
	bd.ByteWidth	=	sizeof(ConstBuffer);
	bd.BindFlags	=	D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_world = Matrix::Identity;

	
}
//-----------------------------------------------------------------------------------
//	SetRenderState()
//	カメラ、ビュー、射影の反映
//	IN	:	カメラ		DirectX::SimpleMath::Vector3 camera
//			ビュー行列	DirectX::SimpleMath::Matrix view
//			射影行列	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void PlayerShaderSample::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

void PlayerShaderSample::Initialize(float life, Vector3 pos, Vector3 velocity)
{
	m_startPosition = pos;
	m_startVelocity = velocity;
	m_startLife = life;

	m_scale = Vector3::Zero;
	m_startScale = Vector3{ 1.0,1.0,1.0 };
	m_endScale = Vector3{ 2.0,2.0,2.0 };

	int randomNum = 5;

	//m_startPosition = Vector3(0.0f, 0.0f, 0.0f);

	m_startPosition = Vector3(0.0f, 1.0f, 6.5f);

	Reset();

	int seed = 100;
	m_wait = ((rand() % seed) / (float)seed) * m_startLife ;
	//m_wait = 0.01;
	//m_gravity = Vector3(0, 0.001f, 0);
}

void PlayerShaderSample::Reset()
{
	m_position = m_startPosition;
	m_velocity = m_startVelocity;
	m_life = m_startLife;
}
//-----------------------------------------------------------------------------------
//	Update()
//	更新処理
//	IN	:	タイマー		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void PlayerShaderSample::Update(StepTimer timer)
{
	//処理に使う秒速計(1秒で1.0f)を取得する。
	float time = float(m_timer.GetElapsedSeconds());
	m_timer = timer;

	m_wait -= time;

	if (m_wait > 0)
	{
		return;
	}

	

	// スケール
	//m_scale = Vector3::Lerp(m_startScale, m_endScale, 1.f - m_life / m_startLife);
	m_scale = Vector3(2.0f, 2.0f, 2.0f);

	m_life -= time;

	Vector3 len = m_position - m_startPosition;

	if (m_life < 0.0f)
	{
		Reset();
	}

}
//-----------------------------------------------------------------------------------
//	Render()
//	描画処理(メイン以外の外部要因）
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void PlayerShaderSample::Render()
{
	if (m_wait > 0) return;
	Matrix scaleMat;
	scaleMat = Matrix::CreateScale(m_scale);
	//ビルボード化
	m_world = scaleMat * Matrix::CreateBillboard(m_position, m_camera, Vector3::UnitY);

	ShaderDraw();
}
//-----------------------------------------------------------------------------------
//	ShaderDraw()
//	メイン描画処理
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void PlayerShaderSample::ShaderDraw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	VertexPositionColorTexture vertex[4] =
	{
	//	VertexPositionColorTexture(Vector3(0.5f, 0.5f, 0.0f),Vector4::One, Vector2(0.0f, 0.0f)),
	//	VertexPositionColorTexture(Vector3(-0.5f, 0.5f, 0.0f),Vector4::One, Vector2(1.0f, 0.0f)),
	//	VertexPositionColorTexture(Vector3(-0.5f, -0.5f, 0.0f),Vector4::One, Vector2(1.0f, 1.0f)),
	//	VertexPositionColorTexture(Vector3(0.5f,-0.5f, 0.0f),Vector4::One, Vector2(0.0f, 1.0f)),
	};

	std::vector<VertexPositionColorTexture> vx;
	for (const Vector3& pos : m_vertexPositionList)
	{
		VertexPositionColorTexture v(Vector3::Zero, Vector4::Zero, Vector2::Zero);
		v.position = pos;

		vx.push_back(v);
	}
	
	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = m_view.Transpose();
	cbuff.matProj = m_proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = Vector4(1, 1, 0, 0);

	/*Vector3 pos = vx[0].position;
	cbuff.Diffuse = Vector4(pos.x, pos.y, pos.z, 1);*/

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);

	// カリングは左周り
	context->RSSetState(m_states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);


	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_batch->Begin();
	//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,&vertex[0],1);
	m_batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
}

void PlayerShaderSample::Draw()
{
}

void PlayerShaderSample::Lost()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_texture2.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}
