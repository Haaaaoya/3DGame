#include "pch.h"
#include "Game/AABBCollider.h"
#include <SimpleMath.h>

AABB::AABB()
{
}

AABB::~AABB()
{
}

bool AABB::AABBCollider(DirectX::SimpleMath::Vector3 a1, DirectX::SimpleMath::Vector3 a2,
	DirectX::SimpleMath::Vector3 b1, DirectX::SimpleMath::Vector3 b2)
{
	if (a1.x <= b2.x &&
		a1.y <= b2.y &&
		a1.z <= b2.z &&
		b1.x >= a2.x &&
		b1.y >= a2.y &&
		b1.z >= a2.z)
	{
		return true;
	}

	return false;
}