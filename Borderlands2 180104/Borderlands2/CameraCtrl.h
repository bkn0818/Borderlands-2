#pragma once
#define g_pCamera CameraCtrl::GetInstance()

class CameraCtrl
{
	SINGLETON(CameraCtrl);

public:
	enum CAMERAKIND
	{
		CAMERA_FIRSTPERSON,
		CAMERA_THIRDPERSON,
		CAMERA_END
	};
public:
	D3DXMATRIX* GetViewMatrix() { return &viewMatrix; }
	D3DXMATRIX* GetBillMatrix() { return &matBill; }
	D3DXMATRIX* GetProjectionMatrix() { return &projMatrix; }
	D3DXMATRIX* GetCamViewMatrix() { return &matView; }

	D3DXMATRIX* SetView(D3DXVECTOR3* pEye, D3DXVECTOR3* pLookAt, D3DXVECTOR3* pUp);
	void		SetupCamera(LPDIRECT3DDEVICE9 pDevice, DWORD zH);

	void RotateLocalX(float fAngle);
	void RotateLocalY(float fAngle);
	void MoveLocalX(float fDist);
	void MoveLocalY(float fDist);
	void MoveLocalZ(float fDist);
	D3DXVECTOR3& GetLookAt() { return vView; }
	D3DXVECTOR3& GetSide() { return vCross; }
	D3DXVECTOR3& GetUpVector() { return upVector; }
	D3DXVECTOR3& GetEyeVector() { return eyeVector; }

	void		ProcessMouse();
	void		ProcessKey();
	void		SetMouseSensitivity(float fSensitivity) { sensitivity = fSensitivity; };	//���콺 �ΰ��� ���� (0.0001 ~ 0.001)
	void		SetEye(D3DXVECTOR3* pEye);						//ī�޶� ��ġ����
	float		GetAngleX() { return angleX; }
	float		GetAngleY() { return angleY; }
	void		SetCamKind(CAMERAKIND kind) { cameraKind = kind; }
	void		WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void		TestPt(bool fire);

private:
	CAMERAKIND		cameraKind = CAMERA_FIRSTPERSON;
	D3DXVECTOR3		eyeVector;
	D3DXVECTOR3		lookAtVector;
	D3DXVECTOR3		upVector;

	D3DXVECTOR3		vView;				// ī�޶� ���ϴ� �������⺤�� 
	D3DXVECTOR3		vCross;				// ī�޶��� ���麤�� cross( view, Up )

	D3DXMATRIX		matView;			// ī�޶� ��� 
	D3DXMATRIX		matBill;			// ������ ���( ī�޶��� ����� )

	D3DXMATRIX		viewMatrix;			//�� ��Ʈ����
	D3DXMATRIX		projMatrix;			//�������� ��Ʈ����

	float			sensitivity = 0.001f;	//���콺 �ΰ���
	float			moveSpeed = 1.0f;
	float			angleY, angleX;

	//3��Ī ī�޶� ���� ����
	bool			leftMouseFlag = false;
	POINT			prevPos;

	float			fRotX, fRotY;
	bool			isLeft	= false;

	POINT	PrevMousePos;
	LPDIRECT3DDEVICE9 dev;
};

