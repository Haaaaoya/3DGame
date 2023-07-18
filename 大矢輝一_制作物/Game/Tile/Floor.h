#pragma once
#include "ObjectBase.h"
#include <Model.h>
#include "Game/Stage/StageLoad.h"
#include "Game/PlaySceneCamera.h"
//前方宣言
class ObjectManager;

class Floor :public ObjectBase
{
public:
	Floor();
	~Floor();

	void Initialize() override;
	void Update() override;
	void Draw(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) override;

	void SetFootHoldFlag(bool FootHoldFlag) { m_footholdFlag = FootHoldFlag; }
private:
	//床モデル
	std::unique_ptr<DirectX::Model> m_Floor;

	std::vector<bool> m_tileFlag;
	//プレイヤーのポジションを受け取る変数
	DirectX::SimpleMath::Vector3 m_position;

	//タイルを踏み始めたかどうかを判断する
	bool m_footholdFlag;
};