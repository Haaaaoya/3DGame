//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"

// �V�[���̗�
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    STAGECREATE,
    PLAY,
    RESULT,
};



// �O���錾
class DebugCamera;

class ILoadingScreen;
class IScene;

/// <summary>
/// Game�N���X����A���[�U���������𔲂��o�����N���X
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

	// �V�[���֌W����

	// �V�[���̍쐬
	void CreateScene();

	// �V�[���̍폜
	void DeleteScene();

private:

    // �L�[�{�[�h
    std::unique_ptr<DirectX::Keyboard> m_keybord;
    
    // �}�E�X
    std::unique_ptr<DirectX::Mouse> m_mouse;

	// ���̃V�[���ԍ�
	GAME_SCENE m_nextScene;

	// �V�[��
	IScene* m_pScene;

    // ���[�h��� 
    std::unique_ptr<ILoadingScreen> m_loadingScreen;
    //�N���A�������ǂ���
    bool m_clear;
    //�I�����ꂽ�X�e�[�W���󂯎��ϐ�
    int m_stageNum;

private:
    void LoadResources(bool useLoadingScreen = true);
};
