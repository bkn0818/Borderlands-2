#include "StdAfx.h"
#include "GameObject.h"
#include "Action.h"

GameObject::GameObject( void )
	: direction( 0, 0, 0 )
	, position( 0, 0, 0 )
	, action( NULL )
	, prevPosition( 0, 0, 0 )
{
	D3DXMatrixIdentity( &worldMatrix );
}


GameObject::~GameObject( void )
{
	SAFE_RELEASE( action );
}


void GameObject::Update()
{
	if ( action )
	{
		action->Update();
	}

	D3DXMATRIXA16 matT, matR;

	D3DXMatrixTranslation( &matT, position.x, position.y, position.z );

	D3DXVECTOR3 d = position - prevPosition;
	if ( D3DXVec3LengthSq( &d ) > D3DX_16F_EPSILON )
	{
		direction = d;
	}

	D3DXMatrixLookAtLH( &matR,
		&D3DXVECTOR3( 0, 0, 0 ),
		&direction,
		&D3DXVECTOR3( 0, 1, 0 ) );
	D3DXMatrixTranspose( &matR, &matR );

	worldMatrix = matR * matT;

	prevPosition = position;
}

void GameObject::Render()
{

}
