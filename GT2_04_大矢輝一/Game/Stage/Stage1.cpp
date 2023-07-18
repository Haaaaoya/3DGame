#include "pch.h"
#include "Stage1.h"
#include "DeviceResources.h"
#include "Game/GameMain.h"
#include <WICTextureLoader.h>
#include "Libraries/Adx2/Adx2.h"
#include <Helpers/CueSheet_0.h>

// 名前空間の利用
using namespace DirectX;
using namespace DirectX::SimpleMath;

Stage1::Stage1():
	StageBase()
{
	m_tileResetCount = 28;
	m_tileCount.resize(m_tileResetCount);
	m_tileFlag.resize(m_tileResetCount);
	m_effectCount.resize(m_tileResetCount);
	//m_drawPos.resize(3);
	for (int i = 0; i < m_tileResetCount; i++)
	{
		m_tileFlag[i] = false;
	}

	for (int i = 0; i < m_tileResetCount; i++)
	{
		m_tileCount[i] = { 0 };
	}
	for (int i = 0; i < m_tileResetCount; i++)
	{
		m_effectCount[i] = 0;
	}
}

Stage1::~Stage1()
{
	
}

void Stage1::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//ジオメトリックプリミティブ
	m_pGeometricPrimitive = GeometricPrimitive::CreateBox(pDR->GetD3DDeviceContext(), XMFLOAT3(5.0f, 0.5f, 5.0f), true, false);

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	
	CreateEffect();

	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	m_stageLoad = std::make_unique<StageLoad>();

	m_pAABBObjectB = std::make_unique<AABBFor3D>();
	m_pAABBObjectB->Initialize();
}

StageBase::STAGE Stage1::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);
	
	m_CamPos = SimpleMath::Vector3::Transform(m_InitialCamPos, m_rotation);
	m_CamPos += SimpleMath::Vector3(m_position.x, 2.0f, m_position.z);

	//カメラの設定
	m_eye = DirectX::SimpleMath::Vector3(
		m_CamPos
	);
	m_up = DirectX::SimpleMath::Vector3(
		0.0f, 1.0f, 0.0f
	);
	m_target = DirectX::SimpleMath::Vector3(
		m_position
	);
	

	//初期位置に戻って1秒経つとゲームが再開される
	if (m_removeCount >= 1.0f)
	{
		m_allResetFlag = false;
		m_tileReset = false;
		m_removeCount = 0.0f;
		m_allResetCount = 0;
		m_fallFlag = false;
	}

	//落ちた判定
	if (m_position.y < -40.0f)
	{
		m_fallFlag = true;
	}
	//全てのタイルが踏まれる→終了するためのタイマーが作動
	//→一定時間過ぎると終了のフラグが立つ→PlaySceneのフェードアウトが行われる
	if (m_tileCount[1] >= 1 && m_tileCount[2] >= 1 && m_tileCount[3] >= 1 && m_tileCount[8] >= 1 && m_tileCount[9] >= 1 &&
		m_tileCount[10] >= 1 && m_tileCount[15] >= 1 && m_tileCount[16] >= 1 && m_tileCount[17] >= 1)
	{
		m_finishTimer += 1 / 60.0f;
	}
	if (m_finishTimer >= 0.5f)
	{
		m_finish = true;
	}
	if (m_stageChange==true)
	{
		return STAGE::STAGE2;
	}
	
	//マネージャの更新
	m_manager->Update();

	return STAGE::NONE;
}

void Stage1::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	//	変換行列の宣言(ワールド)
	DirectX::SimpleMath::Matrix world;


	//	ワールド行列の初期化(単位行列)
	world = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix floarWorld;
	floarWorld = DirectX::SimpleMath::Matrix::Identity;

	floarWorld *= DirectX::SimpleMath::Matrix::CreateScale(4.0f, 0.15f, 4.0f);

	floarWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(2.3f, 0.0f, 5.0f);

	DirectX::SimpleMath::Matrix tileWorld;

	for (int i = 0; i < 8 ;i++)
	{
		tileWorld = DirectX::SimpleMath::Matrix::Identity;

		tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);

		tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 2.5f * i);
		for (int j = 0; j < 8; j++)
		{
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(2.5f, 0.0f, 0.0f);
			auto hoge = tileWorld.Transpose();
		

			/*m_pAABBObjectB->SetData(DirectX::SimpleMath::Vector3(-4.25f + 2.5f * 1, -0.5f, -1.3f + (2.45f * 2)),
				Vector3(-4.25f + 2.5f * (1 + 1), 0.5f, 1.25f + (2.45f * 2)));
			m_pAABBObjectB->Draw(world, view, projection, DirectX::XMFLOAT4(
				0.0f, 1.0f, 0.0f, 1.0f));*/

			//if (m_objectManager->GetStageLoad()->GetStageData(i,j) == 1)
			//{
			//	m_pTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());

			//	if (AABB(Vector3(m_position.x - 0.2f, m_position.y - 0.5f, m_position.z - 0.2f), Vector3(-4.25f + 2.5f * j, -0.5f, -1.3f + (2.45f * i)),
			//		Vector3(m_position.x + 0.2f, m_position.y - 0.4f, m_position.z + 0.2f), Vector3(-4.25f + 2.5f * (j + 1), 0.5f, 1.25f + (2.45f * i))))
			//	{
			//		//CSVが縦4,横7
			//		m_tileFlag[(static_cast<std::vector<bool, std::allocator<bool>>::size_type>(i) * 7) + j] = true;
			//		//最初に乗っていた足場が消える
			//		m_footholdFlag = true;
			//		if (m_effectCount[(static_cast<std::vector<bool, std::allocator<bool>>::size_type>(i) * 7) + j] == 0 && m_removeCount == 0.0f)
			//		{
			//			//指定したエフェクトを原点で再生する
			//			m_handle = m_manager->Play(m_effect, 1.0f, 1.0f, 1.0f);
			//			ADX2::GetInstance().Play(CRI_CUESHEET_0_決定ボタンを押す33);
			//		}
			//		m_effectCount[(static_cast<std::vector<bool, std::allocator<bool>>::size_type>(i) * 7) + j] = 1;
			//	}
			//}
		}
	}

	//下のif分はタイルを踏んだかどうかを判定した直後のこの場所に書くことで正常に動く
	//===========================================================================================

	//タイルを1つでも踏んでいないとき、プレイヤーは落下する
	//二次元配列かvectorか
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			//if (m_objectManager->GetStageLoad()->GetStageData(i, j) == 1)
			//{

			//}
		}
	}

	if (m_tileFlag[1] == false && m_tileFlag[2] == false && m_tileFlag[3] == false && m_tileFlag[8] == false && m_tileFlag[9] == false &&
		m_tileFlag[10] == false && m_tileFlag[15] == false && m_tileFlag[16] == false && m_tileFlag[17] == false)
	{
		m_dropFlag = true;
	}
	else
	{
		m_dropFlag = false;
	}

	//==========================================================================================

	//タイルの状態が下で書き変わる前に
	//タイルをリセットするべきかどうか判定する関数を呼び出す
	TileReset();

	if (m_tileReset == false)
	{
		if (m_tileCount[1] == 1 || m_tileCount[1] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 0.0f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(5.0f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[1] = false;
		}
		if (m_tileCount[2] == 1 || m_tileCount[2] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 0.0f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(7.5f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[2] = false;
		}

		if (m_tileCount[3] == 1 || m_tileCount[3] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 0.0f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(10.0f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[3] = false;
		}

		if (m_tileCount[8] == 1 || m_tileCount[8] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 2.5f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(5.0f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[8] = false;
		}
		if (m_tileCount[9] == 1 || m_tileCount[9] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 2.5f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(7.5f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[9] = false;
		}
		if (m_tileCount[10] == 1 || m_tileCount[10] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 2.5f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(10.0f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[10] = false;
		}


		if (m_tileCount[15] == 1 || m_tileCount[15] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 5.0f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(5.0f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[15] = false;
		}
		if (m_tileCount[16] == 1 || m_tileCount[16] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 5.0f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(7.5f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[16] = false;
		}
		if (m_tileCount[17] == 1 || m_tileCount[17] == 2)
		{
			tileWorld = DirectX::SimpleMath::Matrix::Identity;
			tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 5.0f);
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(10.0f, 0.0f, 0.0f);
			m_pAfterTile->Draw(context, *m_commonState.get(), tileWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
			m_tileFlag[17] = false;
		}
	}

	//同じタイルを踏んだ時のリセット処理
	if (m_allResetFlag == true)
	{
		m_allResetCount++;

		//タイル関係のフラグやタイルを踏んだ時にエフェクトを発生させる
		//配列変数を初期状態に戻す
		if (m_allResetCount > 240)
		{
			for (int i = 0; i < m_tileResetCount; i++)
			{
				m_tileFlag[i] = false;
			}
			m_tileReset = true;
			m_footholdFlag = false;
			for (int i = 0; i < m_tileResetCount; i++)
			{
				m_tileCount[i] = 0;
			}
			m_removeCount += 1.0f / 60;
			for (int i = 0; i < m_tileResetCount; i++)
			{
				m_effectCount[i] = 0;
			}
		}
	}
	else if (m_fallFlag == true)
	{
		for (int i = 0; i < m_tileResetCount; i++)
		{
			m_tileFlag[i] = false;
		}
		m_tileReset = true;
		for (int i = 0; i < m_tileResetCount; i++)
		{
			m_tileCount[i] = 0;
		}
		m_removeCount += 1.0f / 60;
		for (int i = 0; i < m_tileResetCount; i++)
		{
			m_effectCount[i] = 0;
		}
	}

	//プレイヤーがタイルに乗ったら床の当たり判定が無くなる
	if (m_footholdFlag == false)
	{
		////プレイヤーが初めに乗っている床
		//m_pGeometricPrimitive->Draw(floarWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection(), Colors::White, nullptr, false);

		//m_pAABBObjectB->SetData(DirectX::SimpleMath::Vector3(-7.75f, -0.5f, -5.0f),
		//	Vector3(12.25f, 0.5f, 15.0f));
		//m_pAABBObjectB->Draw(world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection(), DirectX::XMFLOAT4(
		//	0.0f, 1.0f, 0.0f, 1.0f));
		//プレイヤーが初めに乗っている床の当たり判定
		//白い床に乗っていなかった場合落ちる
		/*if (AABB(Vector3(m_position.x - 0.15f, m_position.y - 0.5f, m_position.z - 0.15f), Vector3(-7.75f, -0.5f, -5.0f),
			Vector3(m_position.x + 0.15f, m_position.y - 0.4f, m_position.z + 0.15f), Vector3(12.25f, 0.5f, 16.0f)))
		{
			m_startFootHold = false;
		}
		else
		{
			m_startFootHold = true;
		}*/
	}
	//描画
	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();
}

void Stage1::Finalize()
{
}

bool Stage1::AABB(DirectX::SimpleMath::Vector3 a1, DirectX::SimpleMath::Vector3 a2,
	DirectX::SimpleMath::Vector3 b1, DirectX::SimpleMath::Vector3 b2)
{

	if (a1.x <= b2.x &&
		a1.y <= b2.y &&
		a1.z <= b2.z &&
		b1.x >= a2.x &&
		b1.y >= a2.y &&
		b1.z >= a2.z)
	{
		return true;
	}

	return false;
}