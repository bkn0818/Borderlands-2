#include "StdAfx.h"
#include "FrustomCulling.h"


FrustumCulling::FrustumCulling ( void )
{
	orgProjVertexList.clear ();
	orgProjVertexList.push_back ( D3DXVECTOR3 ( -1, 1, 1 ) );
	orgProjVertexList.push_back ( D3DXVECTOR3 ( 1, 1, 1 ) );
	orgProjVertexList.push_back ( D3DXVECTOR3 ( -1, 1, 0 ) );
	orgProjVertexList.push_back ( D3DXVECTOR3 ( 1, 1, 0 ) );
	orgProjVertexList.push_back ( D3DXVECTOR3 ( -1, -1, 1 ) );
	orgProjVertexList.push_back ( D3DXVECTOR3 ( 1, -1, 1 ) );
	orgProjVertexList.push_back ( D3DXVECTOR3 ( -1, -1, 0 ) );
	orgProjVertexList.push_back ( D3DXVECTOR3 ( 1, -1, 0 ) );

	planeList.resize ( 6 );
}


FrustumCulling::~FrustumCulling ( void )
{
}

void FrustumCulling::Update ()
{
	std::vector<D3DXVECTOR3> worldVertexList ( 8 );

	D3DXMATRIXA16 matProj, matView;
	D3DDEVICE->GetTransform ( D3DTS_PROJECTION, &matProj );
	D3DDEVICE->GetTransform ( D3DTS_VIEW, &matView );

	for ( size_t i = 0; i < worldVertexList.size (); ++i )
	{
		//스크린 공간으로부터 개체 공간에 벡터를 투영 한다.
		D3DXVec3Unproject ( &worldVertexList[i],
			&orgProjVertexList[i],
			nullptr,
			&matProj,
			&matView,
			nullptr );
	}

	//                   ^							
	//           /       |                              /
	//        /			v0					        /
	//     /								     /
	//   /		v1					v2		   /

	D3DXPlaneFromPoints ( &planeList[0],
		&worldVertexList[2],
		&worldVertexList[3],
		&worldVertexList[6] );

	D3DXPlaneFromPoints ( &planeList[1],
		&worldVertexList[1],
		&worldVertexList[0],
		&worldVertexList[5] );

	D3DXPlaneFromPoints ( &planeList[2],
		&worldVertexList[0],
		&worldVertexList[2],
		&worldVertexList[4] );

	D3DXPlaneFromPoints ( &planeList[3],
		&worldVertexList[3],
		&worldVertexList[1],
		&worldVertexList[7] );

	D3DXPlaneFromPoints ( &planeList[4],
		&worldVertexList[0],
		&worldVertexList[1],
		&worldVertexList[2] );

	D3DXPlaneFromPoints ( &planeList[5],
		&worldVertexList[6],
		&worldVertexList[7],
		&worldVertexList[4] );
}

bool FrustumCulling::IsInFrustum ( SphereInfo* sphereInfo )
{

	// ax + by + cz + d == 0 점이 평면위에 있다
	// ax + by + cz + d > 0 점이 평면 앞에 있다
	// ax + by + cz + d < 0 점이 평면 뒤에 있다

	for each( D3DXPLANE p in planeList )
	{
		// a*x + b*y + c*z + d*1 
		if ( D3DXPlaneDotCoord ( &p, &sphereInfo->vCenter ) > sphereInfo->fRadius )
		{
			sphereInfo->isInFrustum = false;
			return false;
		}
	}
	sphereInfo->isInFrustum = true;
	return true;
}
