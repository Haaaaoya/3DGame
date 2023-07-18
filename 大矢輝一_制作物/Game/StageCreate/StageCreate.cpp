#include "pch.h"
#include "StageCreate.h"
#include <WICTextureLoader.h>
#include "DeviceResources.h"

#define MAXDATA (8)
StageCreate::StageCreate() :
	m_showPosition(0, 0),
	m_alpha(1.0f),
	m_alphaFlag(true),
	m_framePosition(400, 500),
	m_choiceFlag(false),
	m_saveFlag(false),
	m_yesPosition{ 420,500 },
	m_noPosition{ 820,500 },
	m_moveTimer(0.0f)
{}

StageCreate::~StageCreate()
{
	m_stageLoad.reset();
}

void StageCreate::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_stageData[j][i] = 0;
		}
	}
	//int型で初期化
	m_stageData[static_cast<int>(m_showPosition.x)][static_cast<int>(m_showPosition.y)];

	m_stageLoad = std::make_unique<StageLoad>();
	m_stageLoad->LoadStageData("Resources/StageData/StageCreate.csv");
	//CSVデータを読み込む
	m_stageLoad->readCSV("Resources/StageData/StageCreate.csv", m_csvData);
	//CSVデータを初期化する
	StageDataReset();

	ResourcesLoad::GetInstance()->StageCreateTextureLoad();
}

GAME_SCENE StageCreate::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);

	//アルファ値の変動
	if (m_alphaFlag == true)
	{
		m_alpha += 0.02f;
	}
	else
	{
		m_alpha -= 0.02f;
	}

	//アルファ値の制限
	if (m_alpha > 1.0f)
	{
		m_alpha = 1.0f;
		m_alphaFlag = false;
	}
	if (m_alpha < 0.0f)
	{
		m_alpha = 0.0f;
		m_alphaFlag = true;
	}

	//選択しているタイルの位置を移動
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		m_showPosition.x += 1;
	}
	else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		m_showPosition.x -= 1;
	}
	else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up))
	{
		m_showPosition.y -= 1;
	}
	else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Down))
	{
		m_showPosition.y += 1;
	}
	//点滅している画像の位置
	m_showPosition = Clamp(m_showPosition, DirectX::SimpleMath::Vector2(0, 0), DirectX::SimpleMath::Vector2(7,7));

	//空欄に書き換え
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::D3))
	{
		m_stageData[static_cast<int>(m_showPosition.x)][static_cast<int>(m_showPosition.y)] = 0;
		m_csvData[m_showPosition.y][m_showPosition.x] = 0;
	}
	//タイルに書き換え
	else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::D1))
	{
		m_stageData[static_cast<int>(m_showPosition.x)][static_cast<int>(m_showPosition.y)] = 1;
		m_csvData[m_showPosition.y][m_showPosition.x] = 1;
	}
	//消える床に書き換え
	else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::D2))
	{
		m_stageData[static_cast<int>(m_showPosition.x)][static_cast<int>(m_showPosition.y)] = 2;
		m_csvData[m_showPosition.y][m_showPosition.x] = 2;
	}
	//保存
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::X) && m_saveFlag == true && m_choiceFlag == false)
	{
		// 書き換えた2次元配列をCSVファイルに書き込む
		m_stageLoad->writeCSV("Resources/StageData/StageCreate.csv", m_csvData);
		return GAME_SCENE::TITLE;
	}

	//std::map<DirectX::Keyboard, 関数ポインタ>

	//for ()
	//{
	//	if (IsKeyPressed(key))
	//		map[key]();
	//}


	//セーブ画面に移動
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::X))
	{
		m_saveFlag = true;
	}
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::W))
	{
		m_stageLoad->writeCSV("Resources/StageData/StageCreate.csv", m_csvData);
	}
	//保存する画面の時
	//m_choiceFlagがfalseの時は、フレームが「はい」を指している
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right) && m_saveFlag == true)
	{
		m_choiceFlag = true;
		m_moveTimer = 0;
	}
	else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left) && m_saveFlag == true)
	{
		m_choiceFlag = false;
		m_moveTimer = 0;
	}

	float time = timer.GetElapsedSeconds();
	static const float MOVETIME = 0.4f;
	//選択画像を動かすコード
	//左から右へ移動
	if (m_choiceFlag == true)
	{
		m_moveTimer += time;
		m_framePosition = DirectX::SimpleMath::Vector2::Lerp(m_framePosition, m_noPosition, m_moveTimer / MOVETIME);
	}
	//右から左へ移動
	if (m_choiceFlag == false)
	{
		m_moveTimer += time;
		m_framePosition = DirectX::SimpleMath::Vector2::Lerp(m_framePosition, m_yesPosition, m_moveTimer / MOVETIME);
	}
	//上限を決める
	if (m_moveTimer >= MOVETIME)
	{
		m_moveTimer = MOVETIME;
	}
	return GAME_SCENE::NONE;
}

void StageCreate::Draw()
{
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	for (int i = 0; i < 8 ; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::PaintingBase).Get(), DirectX::SimpleMath::Vector2(250 + 90 * j, -30 + 90 * i), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.3f);

			if (m_stageData[j][i] == 1)
			{
				//(150,0)を基準にX軸,Y軸共に150ずつズレる
				m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Tile).Get(), DirectX::SimpleMath::Vector2(250 + 90 * j, -30 + 90 * i), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.3f);
			}
			else if (m_stageData[j][i] == 2)
			{
				//(150,0)を基準にX軸,Y軸共に150ずつズレる
				m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TransParentFloor).Get(), DirectX::SimpleMath::Vector2(250 + 90 * j, -30 + 90 * i), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.3f);
			}
		}
	}
	//今選択しているマス目を示す画像
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::showTexture).Get(), DirectX::SimpleMath::Vector2(250 + m_showPosition.x * 90, -30 + m_showPosition.y * 90), 0, {1.0f,1.0f,1.0f,m_alpha}, 0.0f, DirectX::g_XMZero, 0.3f);
	//タイル(1キー)を描画するキー画像
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Create1key).Get(), DirectX::SimpleMath::Vector2(1150, 300),
		0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(365, 150),0.3f);

	//床(2キー)を描画するキー画像
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Create2key).Get(), DirectX::SimpleMath::Vector2(1150, 400),
		0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(365, 150), 0.3f);

	//透明を意味する画像(3キー)を描画するキー画像
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Create3key).Get(), DirectX::SimpleMath::Vector2(1150, 500),
		0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(365, 150), 0.3f);

	if (m_saveFlag == true)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::SaveTexture).Get(), DirectX::SimpleMath::Vector2(200, 150), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero);
		//フレーム画像
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::Frame).Get(), m_framePosition,
			0, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2(225, 90),0.8f);
	}

	m_spriteBatch->End();
}

void StageCreate::Finalize()
{}

bool StageCreate::GetClearFlag()
{
	return 0;
}

int StageCreate::GetStageNum()
{
	return 0;
}

void StageCreate::LoadResources()
{}



DirectX::SimpleMath::Vector2 StageCreate::Clamp(const DirectX::SimpleMath::Vector2& a,
	const DirectX::SimpleMath::Vector2& min, const DirectX::SimpleMath::Vector2& max)
{
	DirectX::SimpleMath::Vector2 value = a;
	if (value.x < min.x)
	{
		value.x = min.x;
	}
	else if (value.x > max.x)
	{
		value.x = max.x;
	}

	if (value.y < min.y)
	{
		value.y = min.y;
	}
	else if (value.y > max.y)
	{
		value.y = max.y;
	}
	return value;
}

void StageCreate::StageDataReset()
{
	for (int i = 0; i < MAXDATA; i++)
	{
		for (int j = 0; j < MAXDATA; j++)
		{
			m_csvData[j][i] = 0;
		}
	}
}