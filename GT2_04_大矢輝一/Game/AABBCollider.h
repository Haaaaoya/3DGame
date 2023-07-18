#pragma once
#include <SimpleMath.h>

class AABB
{
private:

public:
	AABB();

	~AABB();

	bool AABBCollider(DirectX::SimpleMath::Vector3 a1, DirectX::SimpleMath::Vector3 a2,
		DirectX::SimpleMath::Vector3 b1, DirectX::SimpleMath::Vector3 b2);
	
		
};