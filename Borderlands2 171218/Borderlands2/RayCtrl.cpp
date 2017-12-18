#include "StdAfx.h"
#include "RayCtrl.h"


RayCtrl::RayCtrl ( void )
	: orgPosition ( 0, 0, 0 )
	, direction ( 0, 0, 0 )
	, raySpaceType ( NONE )
{
}


RayCtrl::~RayCtrl ( void )
{
}

RayCtrl RayCtrl::RayAtViewSpace ( int x, int y )
{
	D3DVIEWPORT9 vp;
	D3DDEVICE->GetViewport ( &vp );

	D3DXMATRIXA16 matProj;
	D3DDEVICE->GetTransform ( D3DTS_PROJECTION, &matProj );

	RayCtrl r;
	r.direction.x = ( ( x * 2.0f ) / vp.Width - 1.0f ) / matProj._11;
	r.direction.y = ( ( -y * 2.0f ) / vp.Height + 1.0f ) / matProj._22;
	r.direction.z = 1.0f;

	D3DXVec3Normalize ( &r.direction, &r.direction );

	r.raySpaceType = VIEW;

	return r;
}

RayCtrl RayCtrl::RayAtWorldSpace ( int x, int y )
{
	D3DXMATRIXA16 matView, matInvView;
	D3DDEVICE->GetTransform ( D3DTS_VIEW, &matView );
	D3DXMatrixInverse ( &matInvView, 0, &matView );

	RayCtrl r = RayCtrl::RayAtViewSpace ( x, y );
	D3DXVec3TransformNormal ( &r.direction, &r.direction, &matInvView );
	D3DXVec3TransformCoord ( &r.orgPosition, &r.orgPosition, &matInvView );

	r.raySpaceType = WORLD;

	return r;
}

bool RayCtrl::IsPicked ( SphereInfo* sphereInfo )
{
	assert ( raySpaceType == WORLD );
	D3DXVECTOR3 localOrg = orgPosition - sphereInfo->vCenter;
	float qv = D3DXVec3Dot ( &localOrg, &direction );
	float vv = D3DXVec3Dot ( &direction, &direction );
	float qq = D3DXVec3Dot ( &localOrg, &localOrg );
	float rr = sphereInfo->fRadius * sphereInfo->fRadius;
	return qv * qv - vv * ( qq - rr ) >= 0;
}
