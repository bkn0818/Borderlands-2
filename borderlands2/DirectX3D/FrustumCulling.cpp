#include "stdafx.h"
#include "FrustumCulling.h"


FrustumCulling::FrustumCulling()
{
}


FrustumCulling::~FrustumCulling()
{
}

void FrustumCulling::Update()
{
	std::vector<D3DXVECTOR3> vList;
	vList.resize(8);

	vList[0] = D3DXVECTOR3(-1, -1, -1);
	vList[1] = D3DXVECTOR3(-1, 1, -1);
	vList[2] = D3DXVECTOR3(1, 1, -1);
	vList[3] = D3DXVECTOR3(1, -1, -1);
	vList[4] = D3DXVECTOR3(-1, -1, 1);
	vList[5] = D3DXVECTOR3(-1, 1, 1);
	vList[6] = D3DXVECTOR3(1, 1, 1);
	vList[7] = D3DXVECTOR3(1, -1, 1);

	D3DXMATRIX matProj, matView;

	D3DDEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DDEVICE->GetTransform(D3DTS_VIEW, &matView);


	orgProjVertexList.resize(vList.size());
	for (int i = 0; i < vList.size(); ++i)
	{
		D3DXVec3Unproject(&orgProjVertexList[i], &vList[i], nullptr, &matProj, &matView, nullptr);
	}
	
	planeList.resize(12);
	D3DXPlaneFromPoints(&planeList[0], &orgProjVertexList[0], &orgProjVertexList[1], &orgProjVertexList[2]);
	D3DXPlaneFromPoints(&planeList[1], &orgProjVertexList[0], &orgProjVertexList[2], &orgProjVertexList[3]);

	D3DXPlaneFromPoints(&planeList[2], &orgProjVertexList[4], &orgProjVertexList[6], &orgProjVertexList[5]);
	D3DXPlaneFromPoints(&planeList[3], &orgProjVertexList[4], &orgProjVertexList[7], &orgProjVertexList[6]);

	D3DXPlaneFromPoints(&planeList[4], &orgProjVertexList[4], &orgProjVertexList[5], &orgProjVertexList[1]);
	D3DXPlaneFromPoints(&planeList[5], &orgProjVertexList[4], &orgProjVertexList[1], &orgProjVertexList[0]);

	D3DXPlaneFromPoints(&planeList[6], &orgProjVertexList[3], &orgProjVertexList[2], &orgProjVertexList[6]);
	D3DXPlaneFromPoints(&planeList[7], &orgProjVertexList[3], &orgProjVertexList[6], &orgProjVertexList[7]);

	D3DXPlaneFromPoints(&planeList[8], &orgProjVertexList[1], &orgProjVertexList[5], &orgProjVertexList[6]);
	D3DXPlaneFromPoints(&planeList[9], &orgProjVertexList[1], &orgProjVertexList[6], &orgProjVertexList[2]);

	D3DXPlaneFromPoints(&planeList[10], &orgProjVertexList[4], &orgProjVertexList[0], &orgProjVertexList[3]);
	D3DXPlaneFromPoints(&planeList[11], &orgProjVertexList[4], &orgProjVertexList[3], &orgProjVertexList[7]);
}


bool FrustumCulling::IsInFrustum(SphereInfo* sphereInfo)
{//
	for each (auto p in planeList)
	{
		if (D3DXPlaneDotCoord(&p, &sphereInfo->vCenter) >= sphereInfo->fRadius)
		{
			return false;
		}
	}
	return true;
}

//프로젝션, 뷰매트릭스를 구하고
//D3DXVec3Unproject(); == 각뿔체인 뷰매트릭스를 월드 매트릭스(육면체)로 바꾼다
//스크린 공간으로부터 개체 공간에 벡터를 투영한다
//D3DXPlaneFromPoints();로 평면 6개를 구해서
//평면과 스피어 포지션의 내적을 구해준다
//ax + by + cz + d == 0 점이 평면 위
//                 > 0 점이 평면 앞
//                 < 0 점이 평면 뒤
//D3DXPlaneDotCoord(); == (ax + by + cz + d)로 평면과 스피어 센터를 계산한 값이
//스피어의 반지름보다 크면 렌더를 안함