#include "stdafx.h"
#include "CharacterCtrl.h"


CharacterCtrl::CharacterCtrl()
	: positionVector(0, 0, 0)
	, rotationY(0)
	, speed(0.1f)
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
	//�̸� �������� �ؽ�ó �浹�� �ƴ��� Ȯ���ϱ� ���� �ӽú���3
	D3DXVECTOR3 temp = positionVector;

	if (GetKeyState('A') & 0x8000) {
		temp = temp - vMoveToSide * speed;
	}

	if (GetKeyState('D') & 0x8000) {
		temp = temp + vMoveToSide * speed;
	}

	D3DXVECTOR3 vDirection(0, 0, -1);

	if (GetKeyState('W') & 0x8000) {
		temp = temp + vMoveToFoward * speed;
	}

	if (GetKeyState('S') & 0x8000) {
		temp = temp - vMoveToFoward * speed;
	}

	if (obj)
	{
		if (obj->GetHeight(temp.x, temp.y, temp.z))
		{
			positionVector = temp;
		}
		else
		{
			temp = positionVector;
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

D3DXVECTOR3 * CharacterCtrl::GetPosition()
{
	return &positionVector;
}

D3DXMATRIX * CharacterCtrl::GetWorldTM()
{
	return &worldMatrix;
}