/*
	プレイシーン
*/
#include "pch.h"

#include <chrono>
#include <thread>

#include "DeviceResources.h"
#include "../GameMain.h"

#include "PlayScene.h"

#include "Libraries/Adx2/Adx2.h"
#include <Helpers/CueSheet_0.h>
#include "Game/Stage/Stage1.h"

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
PlayScene::PlayScene(int stageNumber) :
	//trans(0.0f),
	m_position{ 0.0f,0.0f,0.0f },
	m_InitialCamPos{ 0.0f,2.0f,5.0f },
	m_CamPos{ 0.0f,0.0f,0.0f },
	m_removeCount(0.0f),
	m_footholdFlag(false),
	m_dropFlag(false),
	m_startFootHold(false),
	m_finishTimer(0.0f),
	m_textureTimer(0.0f),
	m_textureFlag(true),
	m_startTileFlag(false),
	m_lifeCount(5),
	m_lifeFlag(false),
	m_effectCount(0),
	m_clear(false),
	m_cameraLeftFlag(false),
	m_cameraRightFlag(false),
	m_manualFlag(1),
	m_cameraLeftCount(0.0f),
	m_cameraRightCount(0.0f),
	m_pStageBase(nullptr),
	m_nextStage(StageBase::STAGE::STAGE1),
	m_stageChange(false)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// カメラ
	m_debugCamera = std::make_unique<DebugCamera>();

	//	AABBボックスを作成する
	m_pAABBObjectA = new AABBFor3D();
	m_pAABBObjectB = new AABBFor3D();

	m_stageNumber = stageNumber;

	ResourcesLoad::GetInstance()->PlayTextureLoad();
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
PlayScene::~PlayScene()
{

	delete m_pAABBObjectB;
	m_pAABBObjectB = nullptr;
	delete m_pAABBObjectA;
	m_pAABBObjectA = nullptr;
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void PlayScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//	スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	DirectX::EffectFactory* factory = new DirectX::EffectFactory(pDR->GetD3DDevice());

	//　テクスチャの読み取りパス指定
	factory->SetDirectory(L"Resources/Models");
	
	m_pBack = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Snow.cmo",
		*factory
	);
	delete factory;

	m_keybordState = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	m_pGeometricPrimitive = DirectX::GeometricPrimitive::CreateBox(pDR->GetD3DDeviceContext(), DirectX::XMFLOAT3(5.0f, 0.5f, 5.0f), true, false);

	//プレイヤーの初期化
	m_player = std::make_unique<Player>();
	m_player->Initialize();
	//終了宣言の初期化
	m_endDeclaration = std::make_unique<EndDeclaration>();
	m_endDeclaration->Initialize();

	m_pBack->UpdateEffects([&](DirectX::IEffect* effect)
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
				// エミッション色を白に設定する
				basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
			}
		});

	//	当たり判定ボックスの初期化
	m_pAABBObjectA->Initialize();
	m_pAABBObjectB->Initialize();

	//	当たり判定ボックスの位置を仮指定
	m_pAABBObjectA->SetData(DirectX::SimpleMath::Vector3(-6.0f, -8.0f, 2.0f), DirectX::SimpleMath::Vector3(-4.0f, -7.0f, 7.0f));
	m_pAABBObjectB->SetData(DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f), DirectX::SimpleMath::Vector3(5.0f, 5.0f, 5.0f));

	m_rect = { 0,0,400,90 };
	//キーボード
	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	ADX2::GetInstance().Initialize("Resources/Sounds/song.acf", "Resources/Sounds/CueSheet_0.acb");

	ADX2::GetInstance().Play(CRI_CUESHEET_0_魔王魂_旧ゲーム音楽_ダンジョン05);

	// 全体の音量の調整（volumeは0.0f～1.0f）
	ADX2::GetInstance().SetVolume(0.4f);

	//プレイシーンカメラの生成
	m_playSceneCamera = std::make_unique<PlaySceneCamera>();

	//オブジェクトの初期化
	m_objectManager = std::make_unique<ObjectManager>();
	m_objectManager->SetPlayer(m_player.get());
	m_objectManager->SetPlaySceneCamera(m_playSceneCamera.get());
	m_objectManager->Initialize(m_stageNumber);

	m_player->SetPlaySceneCamera(m_playSceneCamera.get());

	CreateStage();

	m_pStageBase->SetObjectManager(m_objectManager.get());
	m_pStageBase->SetPlaySceneCamera(m_playSceneCamera.get());
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	int cameraRotation = 3;
	ADX2::GetInstance().Update();

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	
	m_debugCamera->Update();
	
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);

	m_rotation = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle / 180.0f * DirectX::XM_PI);

	// 次のシーンが設定されていたらシーン切り替え
	if (m_nextStage != StageBase::STAGE::NONE)
	{
		// シーン削除
		DeleteStage();

		// シーン作成
		CreateStage();

		m_tileReset = false;
		SetResetPos(false);
		m_textureFlag = true;
	}

	//プレイヤを中心にカメラが回るようにする
	m_CamPos = DirectX::SimpleMath::Vector3::Transform(m_InitialCamPos, m_rotation);
	m_CamPos += DirectX::SimpleMath::Vector3(m_position.x, 2.0f, m_position.z);
	//カメラの位置を変更=======================================================
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::A) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraLeftFlag = true;
	}
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::D) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraRightFlag = true;
	}
	//プレイヤーが動いてる間、カメラは動かせないようにする
	if (m_player->GetInputStopFlag() == true)
	{
		m_cameraLeftFlag = false;
		m_cameraRightFlag = false;
	}

	if (m_cameraLeftFlag == true)
	{
		m_cameraLeftCount++;
		m_angle += 3;
		if (m_cameraLeftCount >= 30)
		{
			m_cameraLeftCount = 0;
			m_cameraLeftFlag = false;
		}
	}

	if (m_cameraRightFlag == true)
	{
		m_cameraRightCount++;
		m_angle -= 3;
		if (m_cameraRightCount >= 30)
		{
			m_cameraRightCount = 0;
			m_cameraRightFlag = false;
		}
	}
	//=========================================================================
	m_InitialCamPos = DirectX::SimpleMath::Vector3(0.0f, 2.0f, 5.0f);

	//カメラ情報を送る、更新する
	m_playSceneCamera->SetMoveTarget(m_position);
	m_playSceneCamera->SetCameraPosition(m_CamPos);
	m_playSceneCamera->SetPositon(m_position);
	m_playSceneCamera->MoveCameraSetting();

	
	m_pStageBase->SetRot(m_rotation);
	m_pStageBase->SetPosition(m_position);
	m_nextStage = m_pStageBase->Update(timer);

	//タイルをリセットするかどうかの結果を受け取る
	m_tileReset = m_pStageBase->GetResetPos();
	m_removeCount = m_pStageBase->GetRemoveCount();
	//終了するフラグをステージに送る
	m_finish = m_pStageBase->GetFinish();

	m_startFootHold = m_pStageBase->GetStartFootHold();
	//同じタイルを踏んだ時に立つフラグを
	//プレイヤークラスに送る
	m_player->SetVelCountReset(m_objectManager->GetAllResetFlag());
	//落ちたかどうか判定するフラグ(m_fallFlag)を
	//プレイヤーに送る
	m_player->SetFallFlag(m_pStageBase->GetFallFlag());

	switch (m_stageNumber)
	{
	case ONE:
		break;
	case TWO:
		//ステージが変わるので、画像の切り抜き位置を変更
		//m_rect = { 0,90,400,180 };
		break;
	case THREE:
		//ステージが変わるので、画像の切り抜き位置を変更
		//m_rect = { 0,180,400,270 };
		break;
	case FOUR:
		//ステージが変わるので、画像の切り抜き位置を変更
		m_rect = { 0,270,400,360 };
		break;
	case FIVE:
		//ステージが変わるので、画像の切り抜き位置を変更
		m_rect = { 0,360,400,450 };
		break;
	}
	
	//画像の透明度を弄る変数のmaxとminを決めておく
	//=====================================================================
	if (m_textureTimer >= 1.0f)
	{
		m_textureTimer = 1.0f;
	}

	if (m_textureTimer <= 0.0f)
	{
		m_textureTimer = 0.0f;
	}
	//=====================================================================
	
	//フェードインアウトの処理
	//=====================================================================
	if (m_finish == true)
	{
		m_finishTimer += 1 / 60.0f;
		m_textureFlag = false;
	}

	//終了する時、プレイヤーも操作できないようにする
	
	//プレイヤークラスにbool型変数を送る
	m_player->SetStartFootHold(m_startFootHold);
	//=====================================================================

	//キャラの移動=============================================================
	
	m_position = m_player->GetPosition();
	m_player->SetTextureFlag(m_textureFlag);
	//足場を踏んでいるかどうかのフラグをPlayerクラスに送る
	m_player->SetFootholdFlag(m_footholdFlag);

	//マネージャーにプレイヤーの位置を渡す
	m_objectManager->SetPosition(m_position);
	//更新
	m_objectManager->Update();

	m_player->Update(timer);
	//プレイヤーが落ちた時にフェードの処理を行う
	if (m_position.y <= -5.0f && m_position.y >= -6.0f)
	{
		m_textureFlag = false;
	}
	if (m_position.y < -40.0f)
	{
		m_textureFlag = true;
	}
	//再び動けるようになるタイマーが1.0を超えたらライフが1減る
	if (m_objectManager->GetRemoveCount() >= 1.0f)
	{
		m_lifeCount--;
	}
	//ライフが0未満になったらゲームオーバー
	if (m_lifeCount < 0)
	{
		m_textureFlag = false;
		return GAME_SCENE::RESULT;
	}
	//ゲームの説明書の切り替え
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		switch (m_manualFlag)
		{
		case 0:
			m_manualFlag = 1;
			break;
		case 1:
			m_manualFlag = 2;
			break;
		case 2:
			m_manualFlag = 0;
			break;
		}
	}
	//オブジェクトを全てリセットした時
	if (m_objectManager->GetAllResetFlag() == true)
	{
		m_endDeclaration->Update(timer);
	}
	//クリア判定を受け取った時、リザルトシーンへ
	if (m_objectManager->GetClearFlag() == true)
	{
		m_finishTimer += 0.1f;
		m_clear = true;
	}
	//終了のタイマーが一定時間経った時
	if (m_finishTimer >= 4.0f)
	{
		return GAME_SCENE::RESULT;
	}

	//フェードの処理
	Fade();

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	//	変換行列の宣言(ワールド、ビュー、射影(プロジェクション))
	DirectX::SimpleMath::Matrix world;

	DirectX::SimpleMath::Matrix celestialSphereWorld, view1;

	//	ワールド行列の初期化(単位行列)
	world = DirectX::SimpleMath::Matrix::Identity;
	/*view = m_pDebugCamera->GetViewMatrix();*/							//	デバッグカメラを使う

	
	// コールバック関数を渡すための変数の設定
	std::function<void()> func = std::bind(&PlayScene::SetState, this);

	//天球のテクスチャ
	m_pBack->Draw(context, *m_commonState.get(), celestialSphereWorld, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());

	m_objectManager->Draw(m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());

	//プレイヤー描画
	m_player->Draw();

	//ステージの描画
	m_pStageBase->Draw();
	m_dropFlag = m_pStageBase->GetDropFlag();
	m_player->SetDropFlag(m_dropFlag);


	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());
	//ステージ数表示
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureStage).Get(), DirectX::SimpleMath::Vector2(0, 0), &m_rect, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 1.0f);

	for (int i = 0; i < m_lifeCount; i++)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureLife).Get(), DirectX::SimpleMath::Vector2(350 + (i * 100), 0), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.2f);
	}

	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureManual).Get(), DirectX::SimpleMath::Vector2(1050, 0), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.1f);
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureSpacekey).Get(), DirectX::SimpleMath::Vector2(900, 0), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 0.5f);

	if (m_manualFlag == 1)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureManual1Page).Get(), DirectX::SimpleMath::Vector2(120, 70), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 1.0f);
	}
	else if(m_manualFlag == 2)
	{
		m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureManual2Page).Get(), DirectX::SimpleMath::Vector2(120, 70), 0, DirectX::Colors::White, 0.0f, DirectX::g_XMZero, 1.0f);
	}

	//フェード用の画像表示
	m_spriteBatch->Draw(ResourcesLoad::GetInstance()->GetTexture(ResourcesLoad::TextureBlack).Get(), DirectX::SimpleMath::Vector2(0, 0), 0, { 1.0f,1.0f,1.0f,m_textureTimer }, 0.0f, DirectX::g_XMZero, 3.0f);

	m_spriteBatch->End();

	m_endDeclaration->MissDraw();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void PlayScene::Finalize()
{
	ADX2::GetInstance().Finalize();
}

void PlayScene::LoadResources()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();

	std::this_thread::sleep_for(std::chrono::seconds(1));
}
void PlayScene::Fade()
{
	if (m_textureFlag == false)
	{
		m_textureTimer += 0.01f;
	}
	else
	{
		m_textureTimer -= 0.01f;
	}
}

void PlayScene::SetState()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	// カリングしない設定
	context->RSSetState(m_commonState->CullNone());
}
bool PlayScene::GetClearFlag()
{
	return m_clear;
}

int PlayScene::GetStageNum()
{
	return 0;
}

void PlayScene::CreateStage()
{
	//ステージが作成されているときは処理しない
	if (m_pStageBase != nullptr)
	{
		return;
	}

	//次のステージ作成
	switch (m_nextStage)
	{
	case StageBase::STAGE::STAGE1:
		m_pStageBase = std::make_unique<Stage1>();
		break;
	default:
		// 例外なので処理を中断
		return;
	}
	m_pStageBase->Initialize();
	
	m_nextStage = StageBase::STAGE::NONE;
}

void PlayScene::DeleteStage()
{
	// ステージが作成されていなければ処理しない
	if (m_pStageBase == nullptr)
	{
		return;
	}

	// 現ステージの終了処理
	m_pStageBase->Finalize();

	// 現ステージの削除
	m_pStageBase.reset();
}