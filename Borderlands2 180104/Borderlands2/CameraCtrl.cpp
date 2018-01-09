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
	//월드 행렬 설정	
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//뷰행렬 설정
	D3DXVECTOR3 vEye(0.0f, 0.0f, -(float)zH);
	D3DXVECTOR3 vLookAt(10.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEye, &vLookAt, &vUp);
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	//프로젝션 행렬 설정
	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI / 2.5f, 1.0f, 0.01f, 10000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &projMatrix);

	//컬링용 프로젝션 행렬
	D3DXMatrixPerspectiveFovLH(&projMatrix, D3DX_PI / 2.5f, 1.0f, 0.1f, 10000.0f);

	//카메라 초기화
	SetView(&vEye, &vLookAt, &vUp);
}

void CameraCtrl::RotateLocalX(float fAngle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &vCross, fAngle);

	D3DXVECTOR3 vNewDst, tempNewDst;
	D3DXVec3TransformCoord(&tempNewDst, &vView, &matRot);

	tempNewDst += eyeVector;
	
	D3DXVECTOR3 tempViewVector;
	D3DXVec3Normalize(&tempViewVector, &(tempNewDst - eyeVector));

	if (D3DXVec3Dot(&tempViewVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)) < 0.9f
		&&D3DXVec3Dot(&tempViewVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f)) > -0.9f)
	{
		D3DXVec3TransformCoord(&vNewDst, &vView, &matRot);
		vNewDst += eyeVector;
		angleX += fAngle;
	}
	else
	{
		vNewDst = lookAtVector;
	}

	SetView(&eyeVector, &vNewDst, &upVector);
}

void CameraCtrl::RotateLocalY(float fAngle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis(&matRot, &upVector, fAngle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &vView, &matRot);

	vNewDst += eyeVector;
	angleY += fAngle;

	SetView(&eyeVector, &vNewDst, &upVector);
}

void CameraCtrl::MoveLocalX(float fDist)
{
	D3DXVECTOR3 vNewEye = eyeVector;
	D3DXVECTOR3 vNewDst = lookAtVector;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &vCross);

	vMove *= fDist;

	vNewEye += vMove;
	vNewDst += vMove;

	SetView(&vNewEye, &vNewDst, &upVector);
}

void CameraCtrl::MoveLocalY(float fDist)
{
	D3DXVECTOR3 vNewEye = eyeVector;
	D3DXVECTOR3 vNewDst = lookAtVector;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &upVector);

	vMove *= fDist;
	vNewEye += vMove;
	vNewDst += vMove;

	SetView(&vNewEye, &vNewDst, &upVector);
}

void CameraCtrl::MoveLocalZ(float fDist)
{
	D3DXVECTOR3 vNewEye = eyeVector;
	D3DXVECTOR3 vNewDst = lookAtVector;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &vView);

	vMove *= fDist;
	vNewEye += vMove;
	vNewDst += vMove;

	SetView(&vNewEye, &vNewDst, &upVector);
}

void CameraCtrl::ProcessMouse()
{
	switch (cameraKind)
	{
		case CAMERA_FIRSTPERSON:
		{
			POINT pt;
			GetCursorPos(&pt);

			//마우스의 변화값
			int dx = pt.x - g_ptMouse.x;
			int dy = pt.y - g_ptMouse.y;

			RotateLocalX(dy * sensitivity);
			RotateLocalY(dx * sensitivity); //마우스의 x축 회전값은 월드에서는 y축 회전값이 된다

			D3DDEVICE->SetTransform(D3DTS_VIEW, &viewMatrix); //카메라 행렬 설정

															  //마우스를 윈도우의 중앙으로 초기화 (에임)
			RECT rect;
			GetClientRect(g_hWnd, &rect);
			pt.x = (float)(rect.right - rect.left) / 2;
			pt.y = (float)(rect.bottom - rect.top) / 2;

			SetCursorPos(pt.x, pt.y);
			ShowCursor(false);

			g_ptMouse.x = pt.x;
			g_ptMouse.y = pt.y;
			break;
		}
		case CAMERA_THIRDPERSON:
		{
			if (g_pKeyManager->IsOnceKeyDown(VK_RBUTTON)) leftMouseFlag = true;
			if (g_pKeyManager->IsOnceKeyUp(VK_RBUTTON)) leftMouseFlag = false;
			if (leftMouseFlag)
			{
				RECT rc;
				GetClientRect(g_hWnd, &rc);
				POINT center;
				center = PointMake((rc.right - rc.left) / 2, (rc.bottom - rc.top) / 2);
				POINT pt;
				GetCursorPos(&pt);

				int dx = pt.x - center.x;
				int dy = pt.y - center.y;

				RotateLocalX(dy * 0.0001f);
				RotateLocalY(dx * 0.0001f);

				D3DDEVICE->SetTransform(D3DTS_VIEW, &viewMatrix);
			}
		}
	}
}

void CameraCtrl::ProcessKey()
{
	switch (cameraKind)
	{
		case CAMERA_FIRSTPERSON:
			if (g_pKeyManager->IsStayKeyDown('A')) MoveLocalX(-moveSpeed);
			if (g_pKeyManager->IsStayKeyDown('D')) MoveLocalX(moveSpeed);
			if (g_pKeyManager->IsStayKeyDown('W')) MoveLocalZ(moveSpeed);
			if (g_pKeyManager->IsStayKeyDown('S')) MoveLocalZ(-moveSpeed);
			break;
		case CAMERA_THIRDPERSON:
			if (g_pKeyManager->IsStayKeyDown(VK_LEFT)) MoveLocalX(-moveSpeed);
			if (g_pKeyManager->IsStayKeyDown(VK_RIGHT)) MoveLocalX(moveSpeed);
			if (g_pKeyManager->IsStayKeyDown(VK_UP)) MoveLocalZ(moveSpeed);
			if (g_pKeyManager->IsStayKeyDown(VK_DOWN)) MoveLocalZ(-moveSpeed);
			if (g_pKeyManager->IsStayKeyDown(VK_LSHIFT)) MoveLocalY(moveSpeed);
			if (g_pKeyManager->IsStayKeyDown(VK_LCONTROL)) MoveLocalY(-moveSpeed);
	D3DDEVICE->SetTransform(D3DTS_VIEW, &viewMatrix);
			break;
	}
}

void CameraCtrl::SetEye(D3DXVECTOR3 * pEye)
{
	eyeVector = *pEye;
}

void CameraCtrl::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
}

void CameraCtrl::TestPt(bool fire)
{
	POINT pt;
	GetCursorPos(&pt);

	//마우스의 변화값
	int dx = pt.x - g_ptMouse.x;
	int dy = pt.y - g_ptMouse.y;

	RotateLocalX(dy * sensitivity);
	RotateLocalY(dx * sensitivity); //마우스의 x축 회전값은 월드에서는 y축 회전값이 된다

	D3DDEVICE->SetTransform(D3DTS_VIEW, &viewMatrix); //카메라 행렬 설정

													  //마우스를 윈도우의 중앙으로 초기화 (에임)
	if (fire)
	{
		float rea;
		
		if (isLeft)
		{
			rea = rand() % 20;
			isLeft = false;
		}
		else
		{
			rea = -(rand() % 20);
			isLeft = true;
		}

		
		SetCursorPos(pt.x + rea, pt.y - 3);
	}
	else
	{
		RECT rect;
		GetClientRect(g_hWnd, &rect);
		pt.x = (float)(rect.right - rect.left) / 2;
		pt.y = (float)(rect.bottom - rect.top) / 2;

		SetCursorPos(pt.x, pt.y);
	}
	
	ShowCursor(false);

	g_ptMouse.x = pt.x;
	g_ptMouse.y = pt.y;
}
