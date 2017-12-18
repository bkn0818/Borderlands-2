#include "stdafx.h"
#include "CameraCtrl.h"

CameraCtrl::CameraCtrl()
{
}

CameraCtrl::~CameraCtrl()
{
}

D3DXMATRIX * CameraCtrl::SetView(D3DXVECTOR3 * pEye, D3DXVECTOR3 * pLookAt, D3DXVECTOR3 * pUp)
{
	eyeVector = *pEye;
	lookAtVector = *pLookAt;
	upVector = *pUp;

	D3DXVec3Normalize(&vView, &(lookAtVector - eyeVector));
	D3DXVec3Cross(&vCross, &upVector, &vView);

	D3DXMatrixLookAtLH(&viewMatrix, &eyeVector, &lookAtVector, &upVector);
	D3DXMatrixInverse(&matBill, nullptr, &viewMatrix);

	matBill._41 = 0.0f;
	matBill._42 = 0.0f;
	matBill._43 = 0.0f;

	return &viewMatrix;
}

void CameraCtrl::SetupCamera(LPDIRECT3DDEVICE9 pDevice, DWORD zH)
{
	//���� ��� ����	
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//����� ����
	D3DXVECTOR3 vEye(0.0f, 0.0f, -(float)zH);
	D3DXVECTOR3 vLookAt(10.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEye, &vLookAt, &vUp);
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	//�������� ��� ����
	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI / 3.0f, 1.0f, 0.1f, 10000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &projMatrix);

	//�ø��� �������� ���
	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI / 4, 1.0f, 1.0f, 2000.0f);

	//ī�޶� �ʱ�ȭ
	this->SetView(&vEye, &vLookAt, &vUp);
}

D3DXMATRIX* CameraCtrl::RotateLocalX(float fAngle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &vCross, fAngle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &vView, &matRot);

	vNewDst += eyeVector;

	return SetView(&eyeVector, &vNewDst, &upVector);
}

D3DXMATRIX* CameraCtrl::RotateLocalY(float fAngle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &upVector, fAngle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &vView, &matRot);

	vNewDst += eyeVector;

	return SetView(&eyeVector, &vNewDst, &upVector);
}

D3DXMATRIX* CameraCtrl::MoveLocalX(float fDist)
{
	D3DXVECTOR3 vNewEye = eyeVector;
	D3DXVECTOR3 vNewDst = lookAtVector;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &vCross);

	vMove *= fDist;

	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &upVector);
}

D3DXMATRIX* CameraCtrl::MoveLocalY(float fDist)
{
	D3DXVECTOR3 vNewEye = eyeVector;
	D3DXVECTOR3 vNewDst = lookAtVector;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &upVector);

	vMove *= fDist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &upVector);
}

D3DXMATRIX* CameraCtrl::MoveLocalZ(float fDist)
{
	D3DXVECTOR3 vNewEye = eyeVector;
	D3DXVECTOR3 vNewDst = lookAtVector;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &vView);

	vMove *= fDist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &upVector);
}

void CameraCtrl::ProcessMouse()
{
	POINT pt;
	float fDelta = sensitivity; //���콺�� �ΰ���, Ŭ���� ���� ������

	GetCursorPos(&pt);

	//���콺�� ��ȭ��
	int dx = pt.x - g_ptMouse.x;
	int dy = pt.y - g_ptMouse.y;

	this->RotateLocalX(dy * fDelta); //���콺�� y�� ȸ������ ���忡���� x�� ȸ������ �ȴ�.
	this->RotateLocalY(dx * fDelta); //���콺�� x�� ȸ������ ���忡���� y�� ȸ������ �ȴ�.

	D3DDEVICE->SetTransform(D3DTS_VIEW, &viewMatrix); //ī�޶� ��� ����

	//���콺�� �������� �߾����� �ʱ�ȭ (����)
	RECT rect;
	GetClientRect(g_hWnd, &rect);
	pt.x = (float)(rect.right - rect.left) / 2;
	pt.y = (float)(rect.bottom - rect.top) / 2;

	SetCursorPos(pt.x, pt.y);
	ShowCursor(false);

	g_ptMouse.x = pt.x;
	g_ptMouse.y = pt.y;
}

void CameraCtrl::ProcessKey()
{
	if (GetKeyState('A') & 0x8000) MoveLocalX(-moveSpeed);
	if (GetKeyState('D') & 0x8000) MoveLocalX(moveSpeed);
	if (GetKeyState('W') & 0x8000) MoveLocalZ(moveSpeed);
	if (GetKeyState('S') & 0x8000) MoveLocalZ(-moveSpeed);
	if (GetKeyState('Q') & 0x8000) MoveLocalY(moveSpeed);
	if (GetKeyState('E') & 0x8000) MoveLocalY(-moveSpeed);
}

void CameraCtrl::SetEye(D3DXVECTOR3 * pEye)
{
	eyeVector = *pEye;
}
