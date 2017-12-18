#pragma once
#define g_pCamera CameraCtrl::GetInstance()

class CameraCtrl
{
	SINGLETON(CameraCtrl);

public:
	D3DXMATRIX* GetViewMatrix() { return &viewMatrix; }
	D3DXMATRIX* GetBillMatrix() { return &matBill; }
	D3DXMATRIX* GetProjectionMatrix() { return &projMatrix; }
	D3DXMATRIX* GetCamViewMatrix() { return &matView; }

	D3DXMATRIX* SetView(D3DXVECTOR3* pEye, D3DXVECTOR3* pLookAt, D3DXVECTOR3* pUp);
	void		SetupCamera(LPDIRECT3DDEVICE9 pDevice, DWORD zH);

	D3DXMATRIX* RotateLocalX(float fAngle);
	D3DXMATRIX* RotateLocalY(float fAngle);
	D3DXMATRIX* MoveLocalX(float fDist);
	D3DXMATRIX* MoveLocalY(float fDist);
	D3DXMATRIX* MoveLocalZ(float fDist);
	D3DXVECTOR3& GetLookAt() { return vView; }
	D3DXVECTOR3& GetSide() { return vCross; }
	D3DXVECTOR3& GetUpVector() { return upVector; }
	D3DXVECTOR3& GetEyeVector() { return eyeVector; }

	void		ProcessMouse();
	void		ProcessKey();
	void		SetMouseSensitivity(float fSensitivity) { sensitivity = fSensitivity; };	//마우스 민감도 설정 (0.0001 ~ 0.001)
	void		SetEye(D3DXVECTOR3* pEye);						//카메라 위치설정

private:
	D3DXVECTOR3		eyeVector;
	D3DXVECTOR3		lookAtVector;
	D3DXVECTOR3		upVector;

	D3DXVECTOR3		vView;				// 카메라가 향하는 단위방향벡터 
	D3DXVECTOR3		vCross;				// 카메라의 측면벡터 cross( view, Up )

	D3DXMATRIX		matView;			// 카메라 행렬 
	D3DXMATRIX		matBill;			// 빌보드 행렬( 카메라의 역행렬 )

	D3DXMATRIX		viewMatrix;			//뷰 매트릭스
	D3DXMATRIX		projMatrix;			//프로젝션 매트릭스

	float			sensitivity = 0.001f;	//마우스 민감도
	float			moveSpeed = 3.0f;

	POINT	PrevMousePos;
	LPDIRECT3DDEVICE9 dev;
};

