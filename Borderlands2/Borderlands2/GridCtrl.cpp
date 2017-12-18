#include "stdafx.h"

#include "Pyramid.h"

#include "GridCtrl.h"


GridCtrl::GridCtrl ()
{
}


GridCtrl::~GridCtrl ()
{
	for ( int i = 0; i < pyramidList.size(); ++i ) {
		delete ( pyramidList[i]);
	}	
	pyramidList.clear ();
}

void GridCtrl::Init ()
{
	int lineCount = 30 / 2;
	float interval = 1.0f;
	float max = lineCount * interval;
	D3DCOLOR	color;


	//AxisXVector = D3DXVECTOR3 ( max, 0, 0 );
	//AxisZVector = D3DXVECTOR3 ( 0, 0, max );
	
	color = D3DCOLOR_XRGB ( 255, 0, 0 );
	vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( -max, 0.0f, 0.0f ), D3DCOLOR ( color ) ) );
	vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( max, 0.0f, 0.0f ), D3DCOLOR ( color ) ) );

	color = D3DCOLOR_XRGB ( 0, 255, 0 );
	vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( 0.0f, -max, 0.0f ), D3DCOLOR ( color ) ) );
	vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( 0.0f, max, 0.0f ), D3DCOLOR ( color ) ) );

	color = D3DCOLOR_XRGB ( 0, 0, 255 );
	vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( 0.0f, 0.0f, -max ), D3DCOLOR ( color ) ) );
	vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( 0.0f, 0.0f, max ), D3DCOLOR ( color ) ) );

	for ( int i = 1; i <= lineCount; ++i ) {

		color = ( i % 5 == 0) ? D3DCOLOR_XRGB ( 255, 255, 255 ) : D3DCOLOR_XRGB ( 128, 128, 128 );
		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( -max, 0.0f, i ), D3DCOLOR ( color ) ) );
		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( max, 0.0f, i ), D3DCOLOR ( color ) ) );

		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( -max, 0.0f, -i ), D3DCOLOR ( color ) ) );
		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( max, 0.0f, -i ), D3DCOLOR ( color ) ) );

		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( i, 0.0f, -max ), D3DCOLOR ( color ) ) );
		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( i, 0.0f, max ), D3DCOLOR ( color ) ) );

		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( -i, 0.0f, -max ), D3DCOLOR ( color ) ) );
		vertexList.push_back ( VertexPC ( D3DXVECTOR3 ( -i, 0.0f, max ), D3DCOLOR ( color ) ) );
	}


	D3DXMATRIXA16 matS, matR, matWorld;
	D3DXMatrixScaling ( &matS, 0.1f, 2.0f, 0.1f );

	Pyramid* pyramid;

	pyramid = new Pyramid;
	D3DXMatrixRotationZ ( &matWorld, D3DX_PI / 2.0f );
	matWorld = matS * matWorld;
	//pyramid->Init ( D3DCOLOR_XRGB ( 255, 0, 0 ), matWorld );
	pyramid->Init ( D3DXCOLOR ( 0.7f, 0.0f, 0.0f, 1.0f ), matWorld );
	pyramidList.push_back ( pyramid );

	pyramid = new Pyramid;
	D3DXMatrixRotationX ( &matWorld, D3DX_PI);
	matWorld = matS * matWorld;
	//pyramid->Init ( D3DCOLOR_XRGB ( 0, 255, 0 ), matWorld );
	pyramid->Init ( D3DXCOLOR ( 0.0f, 0.7f, 0.0f, 1.0f ), matWorld );
	pyramidList.push_back ( pyramid );

	pyramid = new Pyramid;
	D3DXMatrixRotationX ( &matWorld, -D3DX_PI / 2.0f );
	matWorld = matS * matWorld;
	//pyramid->Init ( D3DCOLOR_XRGB ( 0, 0, 255 ), matWorld );
	pyramid->Init ( D3DXCOLOR ( 0.0f, 0.0f, 0.7f, 1.0f ), matWorld );
	pyramidList.push_back ( pyramid );


}

void GridCtrl::Render ()
{

	D3DDEVICE->SetRenderState ( D3DRS_LIGHTING, false );

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity ( &mat );
	
	D3DDEVICE->SetTexture ( 0, NULL );
	D3DDEVICE->SetTransform ( D3DTS_WORLD, &mat );
	D3DDEVICE->SetFVF ( VertexPC::FVF );
	D3DDEVICE->DrawPrimitiveUP (
		D3DPT_LINELIST,
		vertexList.size() / 2,
		&(vertexList[0]),
		sizeof( VertexPC )
		);


	for each ( auto p in pyramidList )
	{
		p->Render ();
	}

}
