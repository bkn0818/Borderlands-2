#include "stdafx.h"
#include "RayCtrl.h"

RayCtrl::RayCtrl()
{
}

RayCtrl::~RayCtrl()
{
}

RayCtrl RayCtrl::RayAtViewSpace(int x, int y)
{
	//뷰포트 가져오기
	D3DVIEWPORT9 vp;
	D3DDEVICE->GetViewport(&vp);
	//프로젝션 매트릭스 가져오기
	D3DXMATRIXA16 proj;
	D3DDEVICE->GetTransform(D3DTS_PROJECTION, &proj);
	//마우스 좌표를 프로젝션으로 변환
	float px = 0;
	float py = 0;

	//용책 348쪽 공식
	px = (((2.0f * x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f * y) / vp.Height) + 1.0f) / proj(1, 1);


	RayCtrl r;
	r.orgPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	r.direction = D3DXVECTOR3(px, py, 1.0f);

	return r;
}

RayCtrl RayCtrl::RayAtWorldSpace(int x, int y)
{
	//광선을 월드스페이스로 변환
	D3DXMATRIXA16 matView, matInvView;
	D3DDEVICE->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);

	RayCtrl r = RayAtViewSpace(x, y);
	D3DXVec3TransformCoord(&r.orgPosition, &r.orgPosition, &matInvView);
	D3DXVec3TransformNormal(&r.direction, &r.direction, &matInvView);

	return r;
}

bool RayCtrl::IsPicked(SphereInfo* sphereInfo)
{
	D3DXVECTOR3 v = orgPosition - sphereInfo->vCenter;

	//V Dot V
	float a = D3DXVec3Dot(&direction, &direction);
	//2 * Q Dot V
	float b = 2.0f * D3DXVec3Dot(&v, &direction);
	//Q Dot Q - r^2
	float c = D3DXVec3Dot(&v, &v) - (sphereInfo->fRadius * sphereInfo->fRadius);
	//판별식
	float disc = (b * b) - (4.0f * a * c);
	
	return disc >= 0;
}
