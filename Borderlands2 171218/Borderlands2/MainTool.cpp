#include "stdafx.h"
#include "MainTool.h"


Control::Control()
{
	this->g_pD3D = NULL;
	this->g_pd3dDevice = NULL;

	this->pFrustum = NULL;
	this->pPick = NULL;

	this->MouseX = 0;
	this->MouseY = 0;

	this->m_Lflag = false;

	this->m_lpstr = ".\\Texture\\HeightMap\\height129.bmp";

	this->vScale.x = 1.0f;
	this->vScale.y = 1.0f;
	this->vScale.z = 1.0f;

	this->m_fLODRatio = 0.005f;
}

HRESULT Control::InitD3D(HWND hWnd)
{
	this->g_hWnd = hWnd;

	//g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //컬링 끄기
	D3DDEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); //컬링 켜기(CCW)
	D3DDEVICE->SetRenderState(D3DRS_ZENABLE, TRUE); //z버퍼 기능 켜기
	
	D3DDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//원본 블랜딩 인수셋팅
	D3DDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//목적지 블랜딩 인수셋팅
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, FALSE); //광원 기능 끄기

	this->pFrustum = new FrustumCulling;
	this->pPick = new Picking;
	this->pPlane = new Plane;

	this->vScale.x = 10.0f;
	this->vScale.z = 10.0f;
	this->vScale.y = 0.5f;

	LPSTR tex = ".\\Texture\\Texture\\Mount.bmp";

	if (FAILED(this->pPlane->InitPlane(&this->vScale, this->m_fLODRatio,
									   this->m_lpstr, tex)))
	{
		return E_FAIL;
	}

	g_pCamera->SetupCamera(D3DDEVICE, this->pPlane->ReturnZH());

	this->InputMouse();

	return S_OK;
}

void Control::Render(bool lflag, bool frflag, bool sFlag)
{
	this->m_Lflag = lflag;

	if (NULL == g_pd3dDevice)
	{
		return;
	}

	//후면 버퍼를 푸른색으로 지운다. z버퍼 초기화
	D3DDEVICE->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	this->Animate(frflag);

	
	this->pPlane->DrawMesh(this->pFrustum, this->pPick, sFlag);
}


void Control::Cleanup()
{
	if (g_pd3dDevice != NULL)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	if (this->pFrustum != NULL)
	{
		delete this->pFrustum;

		this->pFrustum = NULL;
	}

	if (this->pPick != NULL)
	{
		delete this->pPick;

		this->pPick = NULL;
	}

	if (this->pPlane != NULL)
	{
		delete this->pPlane;

		this->pPlane = NULL;
	}
}

void Control::Animate(bool frflag)
{
	this->CameraMove();

	D3DXMATRIX m;
	D3DXMATRIX *pView;
	D3DXMATRIX *pProj;

	pView = g_pCamera->GetViewMatrix();
	pProj = g_pCamera->GetProjectionMatrix();

	m = (*pView) * (*pProj);

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(this->g_hWnd, &pt);

	//this->pPick->CalculatePickingRay(pt.x, pt.y, this->g_pd3dDevice, this->g_hWnd);

	this->pPlane->PickingMap(pPick, frflag);
}

void Control::CameraMove()
{
	
		g_pCamera->ProcessMouse();
		g_pCamera->ProcessKey();
	
}


void Control::InputMouse()
{
	//최초의 마우스 위치 보관
	POINT pt;
	GetCursorPos(&pt);
	this->MouseX = pt.x;
	this->MouseY = pt.y;
}

LPDIRECT3DDEVICE9 Control::ReturnDevice()
{
	return D3DDEVICE;
}

char* Control::ReturnSTR()
{
	return this->m_lpstr;
}

Plane* Control::ReturnPlane()
{
	return this->pPlane;
}

D3DXVECTOR3* Control::ReturnScale()
{
	return &this->vScale;
}

float Control::ReturnLODRatio()
{
	return this->m_fLODRatio;
}