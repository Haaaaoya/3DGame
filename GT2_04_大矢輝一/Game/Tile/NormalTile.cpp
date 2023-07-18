#include "pch.h"
#include "NormalTile.h"
#include "DeviceResources.h"
#include <WICTextureLoader.h>
#include <Effects.h>

const wchar_t* NormalTile::ModelNameTable[NormalTile::ModelNum] =
{
	L"Tile.cmo",
	L"AfterTile.cmo"
};

NormalTile::NormalTile():
	m_footholdFlag (false),
	m_indexX(0),
	m_indexY(0),
	m_allResetFlag(false),
	m_removeCount(0.0f),
	m_fallFlag(false),
	m_allResetCount(0),
	m_tileReset(false),
	m_tileCount(0),
	m_tileNumCount(0),
	m_clearFlag(false)
{
	for (int i = 0; i < 8 ; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_tileFlag[i][j] = false;
			m_tileNum[i][j] = 0;
			m_effectCount[i][j] = 0;
		}
	}
}

NormalTile::~NormalTile()
{

}

void NormalTile::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	for (int i = 0; i < ModelNum; i++)
	{
		//踏む前のタイルのテクスチャの読み取りパス指定
		DirectX::EffectFactory* Tilefactory = new DirectX::EffectFactory(pDR->GetD3DDevice());
		Tilefactory->SetDirectory(L"Resources/Models");

		std::wstring wstr = L"Resources/Models/";
		wstr += ModelNameTable[i];
		m_models[i]= DirectX::Model::CreateFromCMO(
			pDR->GetD3DDevice(),
			wstr.c_str(),
			*Tilefactory
		);
		delete Tilefactory;
	}
}

void NormalTile::Update()
{
	m_position = GetPosition();
	//プレイヤーのポジションの小数点以下を切り捨てる
	m_position.x = round(m_position.x * 10) / 10;
	m_position.z = round(m_position.z * 10) / 10;

	//プレイヤーの現在地を割り出す変数
	int indexX = static_cast<int>((m_position.x + 3.f + 1.25f) / 2.5f);
	int indexY = static_cast<int>((m_position.z + 1.25f) / 2.5f);

	//タイルを踏んだ場合
	if (GetStageLoad()->GetStageData(indexY, indexX) == 1)
	{
		m_footholdFlag = true;
	}
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
	//タイルの処理
	TileProcess();

	//同じタイルを踏んだ時のリセット処理
	if (m_allResetFlag == true)
	{
		//ゲームスタート時に戻すまでのカウントダウン開始
		m_allResetCount++;
		//タイル関係のフラグやタイルを踏んだ時にエフェクトを
		//発生させる配列変数を初期状態に戻す
		if (m_allResetCount > 240)
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					m_tileFlag[i][j] = false;
					m_tileNum[i][j] = 0;
					m_effectCount[i][j] = 0;
				}
			}
			//タイルがリセットされた
			m_tileReset = true;
			//プレイヤーは床の上に戻されるので、タイルに乗った時に
			//立つフラグは下げる
			m_footholdFlag = false;
			//プレイヤーが再び動けるまでのカウント開始
			m_removeCount += 1.0f / 60;
		}
	}
	//プレイヤーが落ちた場合
	else if (m_fallFlag == true)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				m_tileFlag[i][j] = false;
				m_tileNum[i][j] = 0;
				m_effectCount[i][j] = 0;
			}
		}
		//タイルがリセットされた
		m_tileReset = true;
		//プレイヤーは床の上に戻されるので、タイルに乗った時に
		//立つフラグは下げる
		m_footholdFlag = false;
		//プレイヤーが再び動けるまでのカウント開始
		m_removeCount += 1.0f / 60;
	}
}

void NormalTile::Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);
	//床の初期位置
	static const DirectX::SimpleMath::Vector3 TileInitialPosition =
		DirectX::SimpleMath::Vector3(0.21f, 0.25f, 0.21f);

	//カメラ情報の更新
	DirectX::SimpleMath::Matrix tileWorld;
	DirectX::SimpleMath::Matrix afterTileWorld;

	//タイルの数と踏まれたタイルの数を初期化
	m_tileCount = 0;
	m_tileNumCount = 0;
	for (int i = 0; i < 8; i++)
	{
		tileWorld = DirectX::SimpleMath::Matrix::Identity;
		//タイルの大きさを設定
		tileWorld *= DirectX::SimpleMath::Matrix::CreateScale(TileInitialPosition);
		//タイルの位置を設定
		tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-5.5f, 0.0f, 2.5f * i);
		for (int j = 0; j < 8; j++)
		{
			tileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(2.5f, 0.0f, 0.0f);

			//プレイヤーの現在地を割り出す変数
			int indexX = static_cast<int>((m_position.x + 3.f) / 2.5f);
			int indexY = static_cast<int>(m_position.z / 2.5f);
			//CSVのデータが1の時
			if (GetStageLoad()->GetStageData(i, j) == 1)
			{
				//タイルの数を数える
				m_tileCount++;
				//タイルの描画
				m_models[Tile]->Draw(context, *GetCommonState(), tileWorld, GetPlaySceneCamera()->GetView(), GetPlaySceneCamera()->GetProjection());

				if(GetStageLoad()->GetStageData(indexY, indexX) == 1)
				{
					//CSVが縦8,横8
					m_tileFlag[indexY][indexX] = true;
				}
			}
			//タイルを踏むと色が変わる
			if (m_tileNum[i][j] == 1 || m_tileNum[i][j] == 2)
			{
				//踏んだ後のタイルのワールド座標
				afterTileWorld = DirectX::SimpleMath::Matrix::Identity;
				afterTileWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.21f, 0.25f, 0.21f);
				afterTileWorld *= DirectX::SimpleMath::Matrix::CreateTranslation(-3.f + 2.5f * j, 0.0f, 2.5f * i);
				//踏んだ後のタイルの描画
				m_models[AfterTile]->Draw(context, *GetCommonState(), afterTileWorld, GetPlaySceneCamera()->GetView(), GetPlaySceneCamera()->GetProjection());
				m_tileFlag[i][j] = false;
				//踏んだタイルの数を数える
				m_tileNumCount++;
			}
		}
	}
	//描画されてるタイルの数と踏んだタイルの数が一致した時
	//クリア扱いになる
	//この条件式は踏んだタイルを数える変数とタイルのマスを数える変数の
	//更新をしたあとに書く必要がある
	if (m_tileCount == m_tileNumCount)
	{
		m_clearFlag = true;
	}
}

void NormalTile::TileProcess()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//タイルが踏まれた時且つ始めた踏んだタイルの場合
			if (m_tileFlag[i][j] == true && m_tileNum[i][j] == 0)
			{
				m_tileNum[i][j] = 1;
			}
			//一度踏んだタイルから離れた時
			if (m_tileFlag[i][j] == false && m_tileNum[i][j] == 1)
			{
				m_tileNum[i][j] = 2;
			}
			//もう一度同じタイルを踏んだ時
			if (m_tileFlag[i][j] == true && m_tileNum[i][j] == 2)
			{
				m_allResetFlag = true;
			}
		}
		
	}
}