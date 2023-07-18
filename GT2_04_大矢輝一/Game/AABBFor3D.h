

#pragma once

#include <CommonStates.h>
#include <PrimitiveBatch.h>
#include <SimpleMath.h>
#include <VertexTypes.h>
#include <Effects.h>

class AABBFor3D
{
private:
public:

private:
	//中心座標
	DirectX::SimpleMath::Vector3 m_center;
	//各辺までの距離
	DirectX::SimpleMath::Vector3 m_r;

	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	//ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayput;

public:

private:
	void DrawBox(DirectX::XMFLOAT4 color);
public:
	AABBFor3D();
	~AABBFor3D();

	void Initialize();
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix
		view, DirectX::SimpleMath::Matrix projection, DirectX::XMFLOAT4 color);
	void SetData(DirectX::SimpleMath::Vector3 minPos,
		DirectX::SimpleMath::Vector3 maxPos);

	bool HitCheck(AABBFor3D* other);
};