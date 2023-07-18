#pragma once
#include "ObjectBase.h"
#include <Model.h>
#include "Game/Stage/StageLoad.h"
#include "Game/PlaySceneCamera.h"
//前方宣言
class ObjectManager;

class NormalTile:public ObjectBase
{
public:
	enum Models
	{
		Tile,
		AfterTile,

		ModelNum
	};

	NormalTile();
	~NormalTile();

	void Initialize() override;
	void Update() override;
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) override;
	//タイルの処理
	void TileProcess();

	//タイルのリセットを知らせる関数
	bool GetTileReset() { return m_tileReset; }
	//get
	//プレイヤーがタイルに乗ったかどうか判定する変数をゲットする関数
	bool GetFootHoldFlag() { return m_footholdFlag; }
	//全てのタイルをリセットしたかどうかをゲットする為の関数
	bool GetAllResetFlag() { return m_allResetFlag; }
	//void SetObjectManager(ObjectManager* objectManager) { m_objectManager = objectManager; }

	float GetRemoveCount() { return m_removeCount; }

	//クリアしたかどうかをゲットする関数
	bool GetClearFlag(){return m_clearFlag;}
private:
	// モデル名テーブル
	static const wchar_t* ModelNameTable[ModelNum];

	// モデル
	std::unique_ptr<DirectX::Model> m_models[ModelNum];
	//プレイヤーのポジションを受け取る変数
	DirectX::SimpleMath::Vector3 m_position;

	//タイルを踏み始めたかどうかを判断する
	bool m_footholdFlag;
	//プレイヤーの現在地を割り出す変数を受け取る
	int m_indexX;
	int m_indexY;

	bool m_tileFlag[8][8];
	int m_tileNum[8][8];

	//タイルをリセットするまでのカウント
	int m_allResetCount;

	//タイルを全てリセットするフラグ
	bool m_allResetFlag;

	//エフェクトを表示するときに使う
	int m_effectCount[8][8];

	//再び動き出せるまでの時間
	float m_removeCount;

	bool m_tileReset;

	//落ちたかどうか判定するフラグ
	bool m_fallFlag;

	//タイルのマスを数える変数
	int m_tileCount;
	//踏まれたタイルの数を数える変数
	int m_tileNumCount;

	//クリアしたかどうかのフラグ
	bool m_clearFlag;
};