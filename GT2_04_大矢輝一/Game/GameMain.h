//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"

// シーンの列挙
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    STAGECREATE,
    PLAY,
    RESULT,
};



// 前方宣言
class DebugCamera;

class ILoadingScreen;
class IScene;

/// <summary>
/// Gameクラスから、ユーザ処理部分を抜き出したクラス
/// </summary>
class GameMain
{
public:
	/*enum Texture : int
	{
		RightBigDoor,
		RightShortDoor,
		LeftBigDoor,
		LeftShortDoor,
		ZKey,
		Frame,
		GameStart,
		StageCreateTexture,
		Afewline,
		StageSelect,
		Arrow,
		TextureBlack,
		TextureStage,
		TextureManual,
		TextureManual1Page,
		TextureManual2Page,
		TextureSpacekey,
		TextureLife,
		PaintingBase,
		Tile,
		showTexture,
		TransParentFloor,
		SaveTexture,
		Create1key,
		Create2key,
		Create3key,
		Clear,
		GameOver,
		Title,

		TextureNum
	};*/


	GameMain();
	~GameMain();

	void Initialize();
	void Update(const DX::StepTimer& timer);
	void Render();
	void Finalize();

	// シーン関係処理

	// シーンの作成
	void CreateScene();

	// シーンの削除
	void DeleteScene();

private:

    // キーボード
    std::unique_ptr<DirectX::Keyboard> m_keybord;
    
    // マウス
    std::unique_ptr<DirectX::Mouse> m_mouse;

	// 次のシーン番号
	GAME_SCENE m_nextScene;

	// シーン
	IScene* m_pScene;

    // ロード画面 
    std::unique_ptr<ILoadingScreen> m_loadingScreen;
    //クリアしたかどうか
    bool m_clear;
    //選択されたステージを受け取る変数
    int m_stageNum;

private:
    void LoadResources(bool useLoadingScreen = true);
};
