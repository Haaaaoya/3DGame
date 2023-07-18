#include "pch.h"
#include "Player.h"
#include "DeviceResources.h"

#include "Game/GameMain.h"
#include <Effects.h>
#include <SimpleMath.h>

// 名前空間の利用
using namespace DirectX;
using namespace DirectX::SimpleMath;

#define PI        (3.14f)
#define DROPSPEED (0.2f)
#define MAX_ANGLE (3)

Player::Player():
	m_position(DirectX::SimpleMath::Vector3(-3.f, 0.5f, 0.f)),
	m_initialPosition(-3.f, 0.5f, 0.f),
	m_resetPosition(false),
	m_footholdFlag(false),
	m_dropFlag(false),
	m_startFootHold(false),
	m_modelTimer(0),
	m_fallFlag(false),
	m_playerAngle(0.0f),
	m_velocityCount(0),
	m_upFlag(false),
	m_underFlag(false),
	m_leftFlag(false),
	m_rightFlag(false),
	m_cameraLeftFlag(false),
	m_cameraRightFlag(false),
	m_cameraLeftCount(0.0f),
	m_cameraRightCount(0.0f),
	m_angle(0),
	m_inputStopFlag(false),
	m_velCountReset(false),
	m_endDecCount(0),
	m_keyFlag{}
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_pDebugCamera = new DebugCamera();
}

Player::~Player()
{
	if (m_pDebugCamera != nullptr)
	{
		delete m_pDebugCamera;
		m_pDebugCamera = nullptr;
	}
}

void Player::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	EffectFactory* factory = new EffectFactory(pDR->GetD3DDevice());

	//　テクスチャの読み取りパス指定
	factory->SetDirectory(L"Resources/Models");

	//　　ファイルを指定してモデルデータ読み込み
	m_pModel = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/normal.cmo",
		*factory
	);

	delete factory;

	EffectFactory* Rightfactory = new EffectFactory(pDR->GetD3DDevice());

	//　テクスチャの読み取りパス指定
	Rightfactory->SetDirectory(L"Resources/Models");

	//　　ファイルを指定してモデルデータ読み込み
	m_pModelRight = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/rightLeg.cmo",
		*Rightfactory
	);

	delete Rightfactory;

	EffectFactory* Leftfactory = new EffectFactory(pDR->GetD3DDevice());

	//　テクスチャの読み取りパス指定
	Leftfactory->SetDirectory(L"Resources/Models");

	//　　ファイルを指定してモデルデータ読み込み
	m_pModelLeft = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/leftLeg.cmo",
		*Leftfactory
	);

	delete Leftfactory;

	EffectFactory* Fallfactory = new EffectFactory(pDR->GetD3DDevice());

	//　テクスチャの読み取りパス指定
	Fallfactory->SetDirectory(L"Resources/Models");

	//　　ファイルを指定してモデルデータ読み込み
	m_pModelFall = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/fallHuman.cmo",
		*Fallfactory
	);

	delete Fallfactory;

	m_effectManager = std::make_unique<PlayerEffectManager>();
	m_effectManager->Create();
	m_effectManager->Initialize(5.0f, Vector3::Zero);

	RECT outputSize = pDR->GetOutputSize();
	UINT backBufferWidth = std::max<UINT>(outputSize.right - outputSize.left, 1);
	UINT backBufferHeight = std::max<UINT>(outputSize.bottom - outputSize.top, 1);

	DirectX::SimpleMath::Matrix view = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(backBufferWidth) / float(backBufferHeight), 0.1f, 100.f);
	DirectX::SimpleMath::Matrix proj= Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	//初期化
	m_proveWorld = DirectX::SimpleMath::Matrix::Identity;

	//	スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	m_keybordStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	// エフェクトファクトリー
	m_fxFactory = std::make_unique<EffectFactory>(device);

	// リソースディレクトリを設定する
	m_fxFactory->SetDirectory(L"Resources/Sdkmesh");

	// SDKMESHモデルを生成する
	m_model = Model::CreateFromSDKMESH(
		device,
		L"Resources/Sdkmesh/Walking.sdkmesh",
		*m_fxFactory,
		DirectX::ModelLoader_Clockwise | DirectX::ModelLoader_IncludeBones
	);

	// アニメーションクラスのインスタンスを生成する
	m_animation = std::make_unique<DX::AnimationSDKMESH>();

	// アニメーションをロードする
	m_animation->Load(L"Resources/Sdkmesh/Walking.sdkmesh_anim");

	// モデルとボーンをバインドする
	m_animation->Bind(*m_model);

	// ボーン配列を生成する
	m_transformArray = DirectX::ModelBone::MakeArray(m_model->bones.size());

	// モデルで使用するすべてのエフェクトを更新
	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		});

}

void Player::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	m_keybordStateTracker->Update(keyState);
	//プレイヤーに重力を加える
	m_position.y -= DROPSPEED;

	// アニメーションの更新
	m_animation->Update(timer.GetElapsedSeconds());

	//小数点第2以下の値が原因で誤動作が起きている為、
	//整数型にしてから小数点以下を切り捨て
	//
	m_position.x = round(m_position.x * 10) / 10;
	m_position.z = round(m_position.z * 10) / 10;
	
	//プレイヤーの現在地を割り出す変数
	int indexX = static_cast<int>((m_position.x + 3.f + 1.25f) / 2.5f);
	int indexY = static_cast<int>((m_position.z + 1.25f) / 2.5f);
	
	//プレイヤーが足場から落ちていないとき
	if (m_fallFlag == false)
	{
		if (m_startFootHold == false)
		{
			//ゲーム開始時にプレイヤーが乗っている足場が消えていないとき、
			//もしくは足場が消えているかつどのタイルも踏んでいないとき
			if (m_stageLoad->GetStageData(indexY, indexX) == 1 ||
				m_stageLoad->GetStageData(indexY, indexX) == 2 && m_footholdFlag == false)
			{
				m_position.y = 0.5f;
			}
		}
	}
	static const float fallLine = -2.9f;
	//これより下に落ちた時、プレイヤーの操作が出来なくなる
	if (m_position.y < fallLine)
	{
		m_fallFlag = true;
	}

	//カメラを左回転させるフラグ
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::A) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraLeftFlag = true;
	}
	//カメラを右回転させるフラグ
	if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::D) && m_cameraLeftCount == 0 && m_cameraRightCount == 0)
	{
		m_cameraRightFlag = true;
	}
	//プレイヤーが動いてる間、カメラは動かせないようにする
	if (m_inputStopFlag == true)
	{
		m_cameraLeftFlag = false;
		m_cameraRightFlag = false;
	}
	//カメラ左回転
	if (m_cameraLeftFlag == true)
	{
		m_cameraLeftCount++;
		m_angle += MAX_ANGLE;
		if (m_cameraLeftCount >= MAX_ANGLE * 10)
		{
			m_cameraLeftCount = 0;
			m_cameraLeftFlag = false;
		}
	}
	//カメラ右回転
	if (m_cameraRightFlag == true)
	{
		m_cameraRightCount++;
		m_angle -= MAX_ANGLE;
		if (m_cameraRightCount >= MAX_ANGLE * 10)
		{
			m_cameraRightCount = 0;
			m_cameraRightFlag = false;
		}
	}
	//値が大きくなりすぎたり小さくなりすぎないようにリセットをする
	if (m_angle == 360 || m_angle == -360)
	{
		m_angle = 0;
	}
	//=========================================================================

	//プレイシーンを介して受け取ったbool型変数がtrueになることで
	//「Miss」の画像演出が終わるまでカウントを続ける変数が動き出す
	if (m_resetPosition == true)
	{
		m_position = m_initialPosition;
	}
	//同じタイルを2度踏んだ時
	if (m_velCountReset == true)
	{
		m_endDecCount++;
	}
	//240ではない理由はstageクラスのm_removeCountの影響で
	//60フレームのズレが発生するため+60の300になっている
	if (m_endDecCount >= 300)
	{
		//プレイヤーの位置を初期化
		m_position = m_initialPosition;
		m_endDecCount = 0;

		m_velocityCount = 25;
	}

	if (m_fallFlag == true)
	{
		m_velocityCount = 25;
	}
	if (m_velocityCount >= 25)
	{
		//歩き終わったのでフラグを下げる
		m_inputStopFlag = false;
		//プレイヤーの全ての行動のフラグを下げる
		m_upFlag = false;
		m_underFlag = false;
		m_rightFlag = false;
		m_leftFlag = false;
		m_velocityCount = 0;
	}

	m_effectManager->SetRenderState(m_pDebugCamera->GetEyePosition(), m_pDebugCamera->GetViewMatrix(), m_pDebugCamera->GetProjectionMatrix());

	m_effectManager->Update(timer);
}

void Player::Draw()
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//	変換行列の宣言(ワールド、ビュー、射影(プロジェクション))
	DirectX::SimpleMath::Matrix world;
	
	//	ワールド行列の初期化(単位行列)
	world = DirectX::SimpleMath::Matrix::Identity;
	//keyFlagをクリアする
	if (m_inputStopFlag == false)
	{
		ClearKeyFlag();
	}
	//プレイヤーが動ける場合
	if (PlayerMove() == true)
	{
		//	前
		if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_inputStopFlag==false)
		{
			m_upFlag = true;
			m_keyFlag[UP] = true;
		}
		//	左
		else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left) && m_inputStopFlag == false)
		{
			m_leftFlag = true;
			m_keyFlag[LEFT] = true;
		}
		//	後
		else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Down) && m_inputStopFlag == false)
		{
			m_underFlag = true;
			m_keyFlag[DOWN] = true;
		}
		//	右
		else if (m_keybordStateTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right) && m_inputStopFlag == false)
		{
			m_rightFlag = true;
			m_keyFlag[RIGHT] = true;
		}
		//====================================================================================
		//ある方向キーが押された時、
		//カメラの向きに応じてプレイヤーが進む向きを変更する
		//0以上はカメラを右回転させたときの値
		//0以下はカメラを左回転させたときの値
		float charaVel = 0.1f;

		if (m_keyFlag[UP] == true)
		{
			m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
			//キャラが動いてる最中、他の力を加えさせない
			m_inputStopFlag = true;
			
			switch (m_angle)
			{
			case 0:
				m_position.z -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
				break;
			case 90:
			case -270:
				m_position.x -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
				break;
			case 180:
			case -180:
				m_position.z += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
				break;
			case 270:
			case -90:
				m_position.x += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(270));
				break;
			}
			m_velocityCount++;
			//3つのキャラクターモデルを切り替えるタイマー
			m_modelTimer++;
		}
		else if (m_keyFlag[DOWN] == true)
		{
			m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
			//キャラが動いてる最中、他の力を加えさせない
			m_inputStopFlag = true;
			switch (m_angle)
			{
			case 0:
				m_position.z += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(180.0f * PI / 180.0f);
				break;
			case 90:
			case -270:
				m_position.x += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(270.0f * PI / 180.0f);
				break;
			case 180:
			case -180:
				m_position.z -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(0.0f * PI / 180.0f);
				break;
			case 270:
			case -90:
				m_position.x -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * PI);
				break;
			}
			m_velocityCount++;
			//3つのキャラクターモデルを切り替えるタイマー
			m_modelTimer++;
		}
		else if (m_keyFlag[RIGHT] == true)
		{
			m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
			//キャラが動いてる最中、他の力を加えさせない
			m_inputStopFlag = true;
			switch (m_angle)
			{
			case 0:
				m_position.x += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(270.0f * PI / 180.0f);
				break;
			case 90:
			case -270:
				m_position.z -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(0.0f * PI / 180.0f);
				break;
			case 180:
			case -180:
				m_position.x -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * PI);
				break;
			case 270:
			case -90:
				m_position.z += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(180.0f * PI / 180.0f);
				break;
			}
			m_velocityCount++;
			//3つのキャラクターモデルを切り替えるタイマー
			m_modelTimer++;
		}
		else if (m_keyFlag[LEFT] == true)
		{
		m_proveWorld = DirectX::SimpleMath::Matrix::Identity;
		//キャラが動いてる最中、他の力を加えさせない
		m_inputStopFlag = true;
			switch (m_angle)
			{
			case 0:
				m_position.x -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * PI);
				break;
			case 90:
			case -270:
				m_position.z += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(180.0f * PI / 180.0f);
				break;
			case 180:
			case -180:
				m_position.x += charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(270.0f / 180.0f * PI);
				break;
			case 270:
			case -90:
				m_position.z -= charaVel;
				//回転
				m_proveWorld *= DirectX::SimpleMath::Matrix::CreateRotationY(0.0f * PI / 180.0f);
				break;
			}
			m_velocityCount++;
			//3つのキャラクターモデルを切り替えるタイマー
			m_modelTimer++;
			
		}
		else
		{
			m_modelTimer = 0;
		}
	}
	// 拡縮
	//world *= DirectX::SimpleMath::Matrix::CreateScale(0.5f, 0.5f, 0.5f);
	world *= DirectX::SimpleMath::Matrix::CreateScale(0.01f, 0.01f, 0.01f);
	//回転情報を保存した変数をworldに代入
	world *= m_proveWorld;
	// 移動
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	if (m_endDecCount >= 100)
	{
		//m_shader->Render();
	}
	
	auto hoge = world.Transpose();

	if (m_modelTimer >= 0 && m_modelTimer < 5)
	{
		// モデル描画
		//m_pModel->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer >= 5 && m_modelTimer < 10)
	{
		//m_pModelLeft->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer >= 10 && m_modelTimer <= 15)
	{
		//m_pModel->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer >= 15 && m_modelTimer <= 20)
	{
		//m_pModelRight->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	if (m_modelTimer == 20)
	{
		m_modelTimer = 0;
	}

	if (m_position.y < 0)
	{
		m_pModelFall->Draw(context, *m_commonState.get(), world, m_playSceneCamera->GetView(), m_playSceneCamera->GetProjection());
	}
	// ボーン配列のサイズを取得する
	size_t nbones = m_model->bones.size();
	// アニメーションにモデル、ボーン数、ボーンを適用する
	m_animation->Apply(*m_model, nbones, m_transformArray.get());

	//	変換行列の宣言(ワールド、ビュー、射影(プロジェクション))
	DirectX::SimpleMath::Matrix AnimationWorld;

	//	ワールド行列の初期化(単位行列)
	AnimationWorld = DirectX::SimpleMath::Matrix::Identity;
	// 拡縮
	AnimationWorld *= DirectX::SimpleMath::Matrix::CreateScale(0.01f, 0.01f, 0.01f);

	// アニメーションの描画
	m_model->DrawSkinned(
		context,
		*m_commonState,
		nbones,
		m_transformArray.get(),
		world,
		m_playSceneCamera->GetView(), 
		m_playSceneCamera->GetProjection()
	);

}
//プレイヤーが動ける条件式
bool Player::PlayerMove()
{
	//1.フェードインアウト中ではないとき
	//2.プレイヤーの位置がリセットされていないとき
	//3.プレイヤーが落ちていない判定になっているとき
	//4.終了宣言が始まって15フレーム以内(15フレームの理由は、プレイヤーが止まるまでの時間)のとき
	//上記全ての条件を満たしたときプレイヤーは動くことができる
	if (m_textureFlag == true
		&& m_resetPosition == false && m_fallFlag == false
		&& m_endDecCount <= 15)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::ClearKeyFlag()
{
	//どの方向キーも入力できる状態にする
	for (int i = 0; i < 4; i++)
	{
		m_keyFlag[i] = false;
	}
}