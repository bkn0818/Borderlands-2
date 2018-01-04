#include "stdafx.h"
#include "CharacterCtrl.h"


CharacterCtrl::CharacterCtrl()
	: positionVector(50, 0, 100)
	, rotationY(0)
	, speed(0.07f)
{
}

CharacterCtrl::~CharacterCtrl()
{
}

void CharacterCtrl::Init()
{
}

void CharacterCtrl::Update(iMap* obj)
{
	D3DXVec3Normalize(&vMoveToFoward, &vMoveToFoward);
	D3DXVec3Normalize(&vMoveToSide, &vMoveToSide);
	//미리 움직여서 텍스처 충돌이 됐는지 확인하기 위한 임시벡터3
	D3DXVECTOR3 temp = positionVector;

	if (g_pKeyManager->IsStayKeyDown('A')) {
		temp -= (vMoveToSide * speed);
	}

	if (g_pKeyManager->IsStayKeyDown('D')) {
		temp += (vMoveToSide * speed);
	}

	D3DXVECTOR3 vDirection(0, 0, -1);
	D3DXVec3Normalize(&vDirection, &vDirection);

	if (g_pKeyManager->IsStayKeyDown('W')) {
		temp += vMoveToFoward * speed;
	}

	if (g_pKeyManager->IsStayKeyDown('S')) {
		temp -= vMoveToFoward * speed;
	}

	if (obj)
	{
		if (obj->GetHeight(temp.x, temp.y, temp.z))
		{
			positionVector = temp;
		}
	}
	else positionVector = temp;

	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, positionVector.x, positionVector.y, positionVector.z);
	worldMatrix = translationMatrix;
}

void CharacterCtrl::GetCameraLookAt(D3DXVECTOR3& lookAt, D3DXVECTOR3& side)
{
	vMoveToFoward = lookAt;
	D3DXVec3Normalize(&vMoveToFoward, &vMoveToFoward);
	vMoveToSide = side;
	D3DXVec3Normalize(&vMoveToSide, &vMoveToSide);
}

D3DXVECTOR3& CharacterCtrl::GetPosition()
{
	return positionVector;
}

D3DXMATRIX& CharacterCtrl::GetWorldTM()
{
	return worldMatrix;
}
