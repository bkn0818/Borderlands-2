#include "stdafx.h"
#include "CubeCtrl.h"


CubeCtrl::CubeCtrl ()
{
	ZeroMemory ( &material, sizeof ( D3DMATERIAL9 ) );
}


CubeCtrl::~CubeCtrl ()
{
}

void CubeCtrl::Init ( D3DXMATRIXA16* pmat )
{
	std::vector<D3DXVECTOR3> initVerts;
	initVerts.push_back ( D3DXVECTOR3 ( -1.0f, -1.0f, -1.0f ) );
	initVerts.push_back ( D3DXVECTOR3 ( -1.0f, 1.0f, -1.0f ) );
	initVerts.push_back ( D3DXVECTOR3 ( 1.0f, 1.0f, -1.0f ) );
	initVerts.push_back ( D3DXVECTOR3 ( 1.0f, -1.0f, -1.0f ) );
	initVerts.push_back ( D3DXVECTOR3 ( -1.0f, -1.0f, 1.0f ) );
	initVerts.push_back ( D3DXVECTOR3 ( -1.0f, 1.0f, 1.0f ) );
	initVerts.push_back ( D3DXVECTOR3 ( 1.0f, 1.0f, 1.0f ) );
	initVerts.push_back ( D3DXVECTOR3 ( 1.0f, -1.0f, 1.0f ) );

	if ( pmat )
	{
		for ( size_t i = 0; i < initVerts.size (); ++i )
		{
			D3DXVec3TransformCoord ( &initVerts[i], &initVerts[i], pmat );
		}
	}




	D3DXVECTOR3 normal;

	normal = D3DXVECTOR3 ( 0, 0, -1 );
	PNVerts.push_back ( VertexPN ( initVerts[0], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[1], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[2], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[0], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[2], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[3], normal ) );

	normal = D3DXVECTOR3 ( 0, 0, 1 );
	PNVerts.push_back ( VertexPN ( initVerts[4], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[6], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[5], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[4], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[7], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[6], normal ) );

	normal = D3DXVECTOR3 ( -1, 0, 0 );
	PNVerts.push_back ( VertexPN ( initVerts[4], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[5], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[1], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[4], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[1], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[0], normal ) );

	normal = D3DXVECTOR3 ( 1, 0, 0 );
	PNVerts.push_back ( VertexPN ( initVerts[3], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[2], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[6], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[3], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[6], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[7], normal ) );

	normal = D3DXVECTOR3 ( 0, 1, 0 );
	PNVerts.push_back ( VertexPN ( initVerts[1], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[5], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[6], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[1], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[6], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[2], normal ) );

	normal = D3DXVECTOR3 ( 0, -1, 0 );
	PNVerts.push_back ( VertexPN ( initVerts[4], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[0], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[3], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[4], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[3], normal ) );
	PNVerts.push_back ( VertexPN ( initVerts[7], normal ) );
}




void CubeCtrl::Render ( D3DXMATRIXA16* pmat  )
{

	if ( pmat )
		worldMatrix = *pmat;


	D3DDEVICE->SetMaterial ( &material );
	D3DDEVICE->SetRenderState ( D3DRS_LIGHTING, true );

	D3DDEVICE->SetTransform ( D3DTS_WORLD, &worldMatrix );
	D3DDEVICE->SetFVF ( VertexPN::FVF );
	D3DDEVICE->DrawPrimitiveUP ( 
		D3DPT_TRIANGLELIST,
		PNVerts.size() / 3,
		&PNVerts[0],
		sizeof( VertexPN )
	);


}

void CubeCtrl::SetMaterialColor ( D3DXCOLOR color )
{
	ZeroMemory ( &material, sizeof ( D3DMATERIAL9 ) );
	material.Ambient = material.Diffuse = material.Specular = color;

}







