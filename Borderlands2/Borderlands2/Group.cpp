#include "stdafx.h"
#include "Group.h"


Group::Group ()
	: materialTexture(nullptr)
{
}


Group::~Group ()
{
	SAFE_RELEASE ( materialTexture );
}

void Group::Render ()
{
	DWORD dwFillMode;
	D3DDEVICE->GetRenderState ( D3DRS_FILLMODE, &dwFillMode );
	if ( dwFillMode == D3DFILL_WIREFRAME )
	{
		D3DDEVICE->SetTexture ( 0, NULL );
	}
	else
	{
		D3DDEVICE->SetTexture ( 0, materialTexture->GetTexture () );
	}
	D3DDEVICE->SetMaterial ( &materialTexture->GetMaterial() );
	D3DDEVICE->SetFVF ( VertexPNT::FVF );
	D3DDEVICE->DrawPrimitiveUP ( D3DPT_TRIANGLELIST,
		PNTVerts.size () / 3,
		&PNTVerts[0],
		sizeof ( VertexPNT ) );
}
