//2022,05,31
//制作者：加藤慎也
//モデルのデータの管理、描画を行うクラス
#pragma once

//エフェクト
#include <Effects.h>
//モデル
#include <Model.h>
//シンプルマス
#include<SimpleMath.h>

//マップ
#include <unordered_map>



class ModelResources
{
public:
	//インスタンスの窓口
	static ModelResources* GetInstance()
	{
		static ModelResources instance;
		return &instance;
	}
private:
	//モデル
	std::unordered_map<wchar_t*, std::unique_ptr<DirectX::Model>> m_models;

private:
	//コンストラクタやデストラクタは外部からアクセスできないように
	ModelResources() = default;
	~ModelResources() = default;

	//コピーコンストラクタ、コピー代入演算子の削除
	ModelResources(const ModelResources&) = delete;
	ModelResources& operator=(const ModelResources&) = delete;

	//ムーブコンストラクタ、代入ムーブ演算子の削除
	ModelResources(const ModelResources&&) = delete;
	ModelResources&& operator=(const ModelResources&&) = delete;

public:
	//モデルのデータをロードしモデルクラスを作成、連想配列に格納する関数
	//引数：
	//登録したいモデルのタグ名
	//読み込むモデルのファイル名(パス)
	//戻り値：無し
	void LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName);

	//モデルのデータをロードしモデルクラスを作成、連想配列に格納する関数
	//引数：
	//登録したいモデルのタグ名
	//読み込むモデルのファイル名(パス)
	//エミッションさせたい色
	//戻り値：無し
	void LoadModelFile(wchar_t* modelTag, wchar_t* modelFileName,const DirectX::SimpleMath::Vector3& emissionColor);


	//タグで指定されたモデルの取得
	//引数：取得したいモデルのタグ
	//戻り値：モデル
	DirectX::Model* GetModelData(wchar_t* modelTag) { return m_models.at(modelTag).get(); };

};