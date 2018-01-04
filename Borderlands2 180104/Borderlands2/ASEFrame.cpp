
#include "StdAfx.h"
#include "ASEFrame.h"
#include "MatrialTexture.h"

ASEFrame::ASEFrame ( void )
	: materialTexture ( NULL )
{
	D3DXMatrixIdentity ( &localTM );
	D3DXMatrixIdentity ( &worldTM );
}

ASEFrame::~ASEFrame ( void )
{
	SAFE_RELEASE ( materialTexture );
}

void ASEFrame::Render ()
{
	if ( materialTexture )
	{
		D3DDEVICE->SetTransform ( D3DTS_WORLD, &worldTM );
		D3DDEVICE->SetTexture ( 0, materialTexture->GetTexture () );
		D3DDEVICE->SetMaterial ( &materialTexture->GetMaterial () );
		D3DDEVICE->SetFVF ( VertexPNT::FVF );
		D3DDEVICE->DrawPrimitiveUP ( D3DPT_TRIANGLELIST,
			PNTVerts.size () / 3,
			&PNTVerts[0],
			sizeof ( VertexPNT ) );
	}

	for each( auto p in childList )
	{
		p->Render ();
	}
}

void ASEFrame::AddChild ( ASEFrame* pChild )
{
	if ( pChild == NULL )
		return;

	SAFE_ADDREF ( pChild );

	childList.push_back ( pChild );
}

void ASEFrame::Destroy ()
{
	for each( auto p in childList )
	{
		p->Destroy ();
	}
	Release ();
}

void ASEFrame::CalcLocalTM ( ASEFrame* pParent )
{
	localTM = worldTM;

	if ( pParent )
	{
		D3DXMATRIXA16 matInvParent;
		D3DXMatrixInverse ( &matInvParent, 0, &pParent->GetWorldTM () );
		localTM *= matInvParent;
	}

	for each( auto p in childList )
	{
		p->CalcLocalTM ( this );
	}
}

void ASEFrame::Update ( int nKeyFrame, D3DXMATRIXA16* pParent )
{
	D3DXMATRIXA16 matLocalT, matLocalR;

	CalcLocalTranslationMatrix ( nKeyFrame, matLocalT );
	CalcLocalRotationMatrix ( nKeyFrame, matLocalR );

	localTM = matLocalR * matLocalT;

	worldTM = localTM;

	if ( pParent )
		worldTM *= *pParent;

	for each( auto p in childList )
	{
		p->Update ( nKeyFrame, &worldTM );
	}
}

void ASEFrame::CalcLocalTranslationMatrix ( IN int nKeyFrame, OUT D3DXMATRIXA16& mat )
{
	D3DXMatrixIdentity ( &mat );
	if ( posTrackList.empty () )
	{
		mat._41 = localTM._41;
		mat._42 = localTM._42;
		mat._43 = localTM._43;
	}
	else if ( nKeyFrame <= posTrackList.front ().n )
	{
		mat._41 = posTrackList.front ().v.x;
		mat._42 = posTrackList.front ().v.y;
		mat._43 = posTrackList.front ().v.z;
	}
	else if ( nKeyFrame >= posTrackList.back ().n )
	{
		mat._41 = posTrackList.back ().v.x;
		mat._42 = posTrackList.back ().v.y;
		mat._43 = posTrackList.back ().v.z;
	}
	else
	{
		int nNext = -1;
		for ( size_t i = 0; i < posTrackList.size (); ++i )
		{
			if ( posTrackList[i].n > nKeyFrame )
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;

		float t = ( nKeyFrame - posTrackList[nPrev].n ) /
			( float ) ( posTrackList[nNext].n - posTrackList[nPrev].n );

		D3DXVECTOR3 v;
		D3DXVec3Lerp ( &v, &posTrackList[nPrev].v, &posTrackList[nNext].v, t );
		mat._41 = v.x;
		mat._42 = v.y;
		mat._43 = v.z;
	}
}

void ASEFrame::CalcLocalRotationMatrix ( IN int nKeyFrame, OUT D3DXMATRIXA16& mat )
{
	D3DXMatrixIdentity ( &mat );
	if ( rotTrackList.empty () )
	{
		mat = localTM;
		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;
	}
	else if ( nKeyFrame <= rotTrackList.front ().n )
	{
		D3DXMatrixRotationQuaternion ( &mat, &rotTrackList.front ().q );
	}
	else if ( nKeyFrame >= rotTrackList.back ().n )
	{
		D3DXMatrixRotationQuaternion ( &mat, &rotTrackList.back ().q );
	}
	else
	{
		int nNext = -1;
		for ( size_t i = 0; i < rotTrackList.size (); ++i )
		{
			if ( rotTrackList[i].n > nKeyFrame )
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;

		float t = ( nKeyFrame - rotTrackList[nPrev].n ) /
			( float ) ( rotTrackList[nNext].n - rotTrackList[nPrev].n );

		D3DXQUATERNION q;
		D3DXQuaternionSlerp ( &q, &rotTrackList[nPrev].q, &rotTrackList[nNext].q, t );
		D3DXMatrixRotationQuaternion ( &mat, &q );
	}
}
