#pragma once
#include <SimpleMath.h>

class PlaySceneCamera
{
public:
	PlaySceneCamera();
	~PlaySceneCamera();

	DirectX::SimpleMath::Vector3 GetMoveTarget() { return m_moveTarget; }
	DirectX::SimpleMath::Vector3 GetFixedTarget() { return m_fixedTarget; }
	DirectX::SimpleMath::Matrix GetView() { return m_view; }
	DirectX::SimpleMath::Matrix GetProjection() { return m_projection; }

	void SetMoveTarget(DirectX::SimpleMath::Vector3 moveTarget) { 
		m_moveTarget = moveTarget; 
	}
	void SetFixedMoveTarget(DirectX::SimpleMath::Vector3 fixedTarget) {
		m_fixedTarget = fixedTarget;
	}

	void MoveCameraSetting();
	void FixedCameraSetting();

	void SetCameraPosition(DirectX::SimpleMath::Vector3 cameraPosition) { m_cameraPosition = cameraPosition; }
	void SetPositon(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	DirectX::SimpleMath::Vector3 GetPositon() { return m_position; }
private:
	//�����Ă��镨�ւ̃^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_moveTarget;
	//�Î~���Ă��镨�ւ̃^�[�Q�b�g
	DirectX::SimpleMath::Vector3 m_fixedTarget;

	//	�ϊ��s��̐錾(�r���[�A�ˉe(�v���W�F�N�V����))
	DirectX::SimpleMath::Matrix m_view, m_projection;

	DirectX::SimpleMath::Vector3 m_cameraPosition;

	DirectX::SimpleMath::Vector3 m_position;
};