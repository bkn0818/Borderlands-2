#include "StdAfx.h"
#include "FrustomCulling.h"


FrustumCulling::FrustumCulling(void)
{
	orgProjVertexList.resize(8);
	planeList.resize(6);
}


FrustumCulling::~FrustumCulling(void)
{
}

void FrustumCulling::Init(D3DXMATRIX * pMatViewProj)
{
	D3DXMATRIX matInv;

	orgProjVertexList.push_back(D3DXVECTOR3(-1, -1, 0));
	orgProjVertexList.push_back(D3DXVECTOR3( 1, -1, 0));
	orgProjVertexList.push_back(D3DXVECTOR3( 1, -1, 1));
	orgProjVertexList.push_back(D3DXVECTOR3(-1, -1, 1));
	orgProjVertexList.push_back(D3DXVECTOR3(-1,  1, 0));
	orgProjVertexList.push_back(D3DXVECTOR3( 1,  1, 0));
	orgProjVertexList.push_back(D3DXVECTOR3( 1,  1, 1));
	orgProjVertexList.push_back(D3DXVECTOR3(-1,  1, 1));

	D3DXMatrixInverse(&matInv, nullptr, pMatViewProj);

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&orgProjVertexList[i], &orgProjVertexList[i], &matInv);
	}

	vPos = (orgProjVertexList[0] + orgProjVertexList[5]) / 2.0f;


	D3DXPlaneFromPoints(&planeList[0],
						&orgProjVertexList[2],
						&orgProjVertexList[3],
						&orgProjVertexList[6]);

	D3DXPlaneFromPoints(&planeList[1],
						&orgProjVertexList[1],
						&orgProjVertexList[0],
						&orgProjVertexList[5]);

	D3DXPlaneFromPoints(&planeList[2],
						&orgProjVertexList[0],
						&orgProjVertexList[2],
						&orgProjVertexList[4]);

	D3DXPlaneFromPoints(&planeList[3],
						&orgProjVertexList[3],
						&orgProjVertexList[1],
						&orgProjVertexList[7]);

	D3DXPlaneFromPoints(&planeList[4],
						&orgProjVertexList[0],
						&orgProjVertexList[1],
						&orgProjVertexList[2]);

	D3DXPlaneFromPoints(&planeList[5],
						&orgProjVertexList[6],
						&orgProjVertexList[7],
						&orgProjVertexList[4]);
}

void FrustumCulling::Update()
{
	std::vector<D3DXVECTOR3> worldVertexList(8);

	D3DXMATRIXA16 matProj, matView;
	D3DDEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DDEVICE->GetTransform(D3DTS_VIEW, &matView);


	//0번과 5번은 절두체중 near평면의 좌측 상단과 우측 하단
	//둘의 좌표를 더해서 2로 나누면, 카메라의 좌표를 얻을 수 있다.(정확히 일치하지는 않음)
	this->vPos = (this->orgProjVertexList[0] + this->orgProjVertexList[5]) / 2.0f;

	for (size_t i = 0; i < worldVertexList.size(); ++i)
	{
		//스크린 공간으로부터 개체 공간에 벡터를 투영 한다.
		D3DXVec3Unproject(&worldVertexList[i],
						  &orgProjVertexList[i],
						  nullptr,
						  &matProj,
						  &matView,
						  nullptr);
	}


}

bool FrustumCulling::IsInFrustum(SphereInfo* sphereInfo)
{
	// ax + by + cz + d == 0 점이 평면위에 있다
	// ax + by + cz + d > 0 점이 평면 앞에 있다
	// ax + by + cz + d < 0 점이 평면 뒤에 있다

	for each(D3DXPLANE p in planeList)
	{
		// a*x + b*y + c*z + d*1 
		if (D3DXPlaneDotCoord(&p, &sphereInfo->vCenter) > sphereInfo->fRadius)
		{
			sphereInfo->isInFrustum = false;
			return false;
		}
	}
	sphereInfo->isInFrustum = true;
	return true;
}

bool FrustumCulling::IsInSphere(D3DXVECTOR3 * pv, float radius)
{
	float fDst;

	fDst = D3DXPlaneDotCoord(&planeList[3], pv);
	if (fDst > (radius + D3DX_16F_EPSILON)) return false;

	fDst = D3DXPlaneDotCoord(&planeList[4], pv);
	if (fDst > (radius + D3DX_16F_EPSILON)) return false;

	fDst = D3DXPlaneDotCoord(&planeList[5], pv);
	if (fDst > (radius + D3DX_16F_EPSILON)) return false;

	return true;
}

bool FrustumCulling::IsIn(D3DXVECTOR3 * pv)
{
	float fDst;

	fDst = D3DXPlaneDotCoord(&planeList[3], pv);
	if (fDst > D3DX_16F_EPSILON) return false;

	fDst = D3DXPlaneDotCoord(&planeList[4], pv);
	if (fDst > D3DX_16F_EPSILON) return false;

	fDst = D3DXPlaneDotCoord(&planeList[5], pv);
	if (fDst > D3DX_16F_EPSILON) return false;

	return true;
}
