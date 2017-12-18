#include "StdAfx.h"

#include "ASEFrame.h"
#include "ASELoader.h"
#include "Asciitok.h"
#include "MatrialTexture.h"


ASELoader::ASELoader ( void )
	: fileStream ( NULL )
	, root ( NULL )
{
}

ASELoader::~ASELoader ( void )
{
}

ASEFrame* ASELoader::Load ( IN char* filename, OUT ASEScene* pScene )
{
	fopen_s ( &fileStream, filename, "r" );

	while ( true )
	{
		char* szToken = GetToken ();
		if ( szToken == NULL )
			break;

		if ( IsEqual ( szToken, ID_SCENE ) )
		{
			if ( pScene )
				ProcessSCENE ( pScene );
		}
		else if ( IsEqual ( szToken, ID_MATERIAL_LIST ) )
		{
			ProcessMATERIAL_LIST ();
		}
		else if ( IsEqual ( szToken, ID_GEOMETRY ) )
		{
			ASEFrame* pFrame = ProcessGEOMOBJECT ();
			if ( root == NULL )
			{
				root = pFrame;
			}
		}
	}

	fclose ( fileStream );

	root->CalcLocalTM ( NULL );
	return root;
}

char* ASELoader::GetToken ()
{
	bool isQuote = false;
	int nReadCount = 0;

	while ( true )
	{
		unsigned char c = fgetc ( fileStream );

		if ( feof ( fileStream ) )
			break;

		if ( !isQuote && IsWhite ( c ) )
		{
			if ( nReadCount == 0 )
				continue;
			else
				break;
		}

		if ( c == '\"' )
		{
			if ( isQuote )
				break;

			isQuote = true;
			continue;
		}

		m_szToken[nReadCount++] = c;
	}

	if ( nReadCount == 0 )
		return NULL;

	m_szToken[nReadCount] = '\0';

	return m_szToken;
}

int ASELoader::GetInteger ()
{
	return atoi ( GetToken () );
}

float ASELoader::GetFloat ()
{
	return ( float ) atof ( GetToken () );
}

bool ASELoader::IsWhite ( char c )
{
	return c < 33;
}

bool ASELoader::IsEqual ( char* str1, char* str2 )
{
	return strcmp ( str1, str2 ) == 0;
}

void ASELoader::SkipBlock ()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessMATERIAL_LIST ()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_MATERIAL_COUNT ) )
		{
			if ( !materialTextureList.empty () )
			{
				for each( auto p in materialTextureList )
					SAFE_RELEASE ( p );
				materialTextureList.clear ();
			}

			materialTextureList.resize ( GetInteger () );
		}
		else if ( IsEqual ( szToken, ID_MATERIAL ) )
		{
			int nMtlRef = GetInteger ();
			SAFE_RELEASE ( materialTextureList[nMtlRef] );
			materialTextureList[nMtlRef] = new MaterialTexture;
			ProcessMATERIAL ( materialTextureList[nMtlRef] );
			materialTextureList[nMtlRef]->AutoRelease ();
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessMATERIAL ( OUT MaterialTexture* pMtlTex )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_AMBIENT ) )
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMaterial ();
			stMtl.Ambient.r = GetFloat ();
			stMtl.Ambient.g = GetFloat ();
			stMtl.Ambient.b = GetFloat ();
			stMtl.Ambient.a = 1.0f;
		}
		else if ( IsEqual ( szToken, ID_DIFFUSE ) )
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMaterial ();
			stMtl.Diffuse.r = GetFloat ();
			stMtl.Diffuse.g = GetFloat ();
			stMtl.Diffuse.b = GetFloat ();
			stMtl.Diffuse.a = 1.0f;
		}
		else if ( IsEqual ( szToken, ID_SPECULAR ) )
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMaterial ();
			stMtl.Specular.r = GetFloat ();
			stMtl.Specular.g = GetFloat ();
			stMtl.Specular.b = GetFloat ();
			stMtl.Specular.a = 1.0f;
		}
		else if ( IsEqual ( szToken, ID_MAP_DIFFUSE ) )
		{
			ProcessMAP_DIFFUSE ( pMtlTex );
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessMAP_DIFFUSE ( OUT MaterialTexture* pMtlTex )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_BITMAP ) )
		{
			pMtlTex->SetTexture ( g_pTextureManager->GetTexture ( GetToken () ) );
		}
	} while ( nLevel > 0 );
}

ASEFrame* ASELoader::ProcessGEOMOBJECT ()
{
	ASEFrame* pFrame = new ASEFrame();

	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_NODE_NAME ) )
		{
			char* szNodeName = GetToken ();
			frameMap[szNodeName] = pFrame;
		}
		else if ( IsEqual ( szToken, ID_NODE_PARENT ) )
		{
			char* szParentNodeName = GetToken ();
			frameMap[szParentNodeName]->AddChild ( pFrame );
		}
		else if ( IsEqual ( szToken, ID_NODE_TM ) )
		{
			ProcessNODE_TM ( pFrame );
		}
		else if ( IsEqual ( szToken, ID_MESH ) )
		{
			ProcessMESH ( pFrame );
		}
		else if ( IsEqual ( szToken, ID_TM_ANIMATION ) )
		{
			ProcessTM_ANIMATION ( pFrame );
		}
		else if ( IsEqual ( szToken, ID_MATERIAL_REF ) )
		{
			pFrame->SetMaterialTexture ( materialTextureList[GetInteger ()] );
		}
	} while ( nLevel > 0 );

	pFrame->AutoRelease ();

	return pFrame;
}

void ASELoader::ProcessMESH ( OUT ASEFrame* pFrame )
{
	std::vector<D3DXVECTOR3>	Vs;
	std::vector<D3DXVECTOR2>	VTs;
	std::vector<VertexPNT>	PNTVerts;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_MESH_NUMVERTEX ) )
		{
			Vs.resize ( GetInteger () );
		}
		else if ( IsEqual ( szToken, ID_MESH_NUMFACES ) )
		{
			PNTVerts.resize ( 3 * GetInteger () );
		}
		else if ( IsEqual ( szToken, ID_MESH_VERTEX_LIST ) )
		{
			ProcessMESH_VERTEX_LIST ( Vs );
		}
		else if ( IsEqual ( szToken, ID_MESH_FACE_LIST ) )
		{
			ProcessMESH_FACE_LIST ( Vs, PNTVerts );
		}
		else if ( IsEqual ( szToken, ID_MESH_NUMTVERTEX ) )
		{
			VTs.resize ( GetInteger () );
		}
		else if ( IsEqual ( szToken, ID_MESH_TVERTLIST ) )
		{
			ProcessMESH_TVERTLIST ( VTs );
		}
		else if ( IsEqual ( szToken, ID_MESH_NUMTVFACES ) )
		{
			GetToken ();
		}
		else if ( IsEqual ( szToken, ID_MESH_TFACELIST ) )
		{
			ProcessMESH_TFACELIST ( VTs, PNTVerts );
		}
		else if ( IsEqual ( szToken, ID_MESH_NORMALS ) )
		{
			ProcessMESH_NORMALS ( PNTVerts );
		}

	} while ( nLevel > 0 );

	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixInverse ( &matInvWorld, 0, &pFrame->GetWorldTM () );
	for ( size_t i = 0; i < PNTVerts.size (); ++i )
	{
		D3DXVec3TransformCoord ( &PNTVerts[i].p, &PNTVerts[i].p, &matInvWorld );
		D3DXVec3TransformNormal ( &PNTVerts[i].n, &PNTVerts[i].n, &matInvWorld );
	}

	pFrame->SetPNTVerts ( PNTVerts );
}

void ASELoader::ProcessMESH_VERTEX_LIST ( OUT std::vector<D3DXVECTOR3>& vecV )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_MESH_VERTEX ) )
		{
			int nIndex = GetInteger ();
			vecV[nIndex].x = GetFloat ();
			vecV[nIndex].z = GetFloat ();
			vecV[nIndex].y = GetFloat ();
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessMESH_FACE_LIST ( IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<VertexPNT>& vecVertex )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_MESH_FACE ) )
		{
			int nFaceIndex = GetInteger ();
			GetToken (); // A:
			int nA = GetInteger ();

			GetToken (); // B:
			int nB = GetInteger ();

			GetToken (); // C:
			int nC = GetInteger ();

			vecVertex[nFaceIndex * 3 + 0].p = vecV[nA];
			vecVertex[nFaceIndex * 3 + 1].p = vecV[nC];
			vecVertex[nFaceIndex * 3 + 2].p = vecV[nB];
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessMESH_TVERTLIST ( OUT std::vector<D3DXVECTOR2>& vecVT )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_MESH_TVERT ) )
		{
			int nIndex = GetInteger ();
			vecVT[nIndex].x = GetFloat ();
			vecVT[nIndex].y = 1.0f - GetFloat ();
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessMESH_TFACELIST ( IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<VertexPNT>& vecVertex )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_MESH_TFACE ) )
		{
			int nFaceIndex = GetInteger ();
			int nA = GetInteger ();
			int nB = GetInteger ();
			int nC = GetInteger ();

			vecVertex[nFaceIndex * 3 + 0].t = vecVT[nA];
			vecVertex[nFaceIndex * 3 + 1].t = vecVT[nC];
			vecVertex[nFaceIndex * 3 + 2].t = vecVT[nB];
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessMESH_NORMALS ( OUT std::vector<VertexPNT>& vecVertex )
{
	int nFaceIndex = 0;
	int nFaceCount = 0;
	int aModCount[] = { 0, 2, 1 };

	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_MESH_FACENORMAL ) )
		{
			nFaceIndex = GetInteger ();
			nFaceCount = 0;
		}
		else if ( IsEqual ( szToken, ID_MESH_VERTEXNORMAL ) )
		{
			GetToken ();
			vecVertex[nFaceIndex * 3 + aModCount[nFaceCount]].n.x = GetFloat ();
			vecVertex[nFaceIndex * 3 + aModCount[nFaceCount]].n.z = GetFloat ();
			vecVertex[nFaceIndex * 3 + aModCount[nFaceCount]].n.y = GetFloat ();
			++nFaceCount;
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessNODE_TM ( OUT ASEFrame* pFrame )
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity ( &matWorld );

	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_TM_ROW0 ) )
		{
			matWorld._11 = GetFloat ();
			matWorld._13 = GetFloat ();
			matWorld._12 = GetFloat ();
			matWorld._14 = 0.0f;
		}
		else if ( IsEqual ( szToken, ID_TM_ROW1 ) )
		{
			matWorld._31 = GetFloat ();
			matWorld._33 = GetFloat ();
			matWorld._32 = GetFloat ();
			matWorld._34 = 0.0f;
		}
		else if ( IsEqual ( szToken, ID_TM_ROW2 ) )
		{
			matWorld._21 = GetFloat ();
			matWorld._23 = GetFloat ();
			matWorld._22 = GetFloat ();
			matWorld._24 = 0.0f;
		}
		else if ( IsEqual ( szToken, ID_TM_ROW3 ) )
		{
			matWorld._41 = GetFloat ();
			matWorld._43 = GetFloat ();
			matWorld._42 = GetFloat ();
			matWorld._44 = 1.0f;
		}
	} while ( nLevel > 0 );

	pFrame->SetWorldTM ( matWorld );
}

void ASELoader::ProcessTM_ANIMATION ( OUT ASEFrame* pFrame )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_POS_TRACK ) )
		{
			ProcessPOS_TRACK ( pFrame );
		}
		else if ( IsEqual ( szToken, ID_ROT_TRACK ) )
		{
			ProcessROT_TRACK ( pFrame );
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessPOS_TRACK ( OUT ASEFrame* pFrame )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_POS_SAMPLE ) )
		{
			PositionSample positionSample;
			positionSample.n = GetInteger ();
			positionSample.v.x = GetFloat ();
			positionSample.v.z = GetFloat ();
			positionSample.v.y = GetFloat ();
			pFrame->posTrackList.push_back ( positionSample );
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessROT_TRACK ( OUT ASEFrame* pFrame )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_ROT_SAMPLE ) )
		{
			RotationSample rotationSample;

			rotationSample.n = GetInteger ();

			rotationSample.q.x = GetFloat ();
			rotationSample.q.z = GetFloat ();
			rotationSample.q.y = GetFloat ();
			rotationSample.q.w = GetFloat ();

			rotationSample.q.x *= sinf ( rotationSample.q.w / 2.0f );
			rotationSample.q.y *= sinf ( rotationSample.q.w / 2.0f );
			rotationSample.q.z *= sinf ( rotationSample.q.w / 2.0f );
			rotationSample.q.w = cosf ( rotationSample.q.w / 2.0f );

			if ( !pFrame->rotTrackList.empty () )
			{
				rotationSample.q = pFrame->rotTrackList.back ().q * rotationSample.q;
			}

			pFrame->rotTrackList.push_back ( rotationSample );
		}
	} while ( nLevel > 0 );
}

void ASELoader::ProcessSCENE ( OUT ASEScene* pScene )
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken ();
		if ( IsEqual ( szToken, "{" ) )
		{
			++nLevel;
		}
		else if ( IsEqual ( szToken, "}" ) )
		{
			--nLevel;
		}
		else if ( IsEqual ( szToken, ID_FIRSTFRAME ) )
		{
			pScene->firstFrame = GetInteger ();
		}
		else if ( IsEqual ( szToken, ID_LASTFRAME ) )
		{
			pScene->lastFrame = GetInteger ();
		}
		else if ( IsEqual ( szToken, ID_FRAMESPEED ) )
		{
			pScene->frameSpeed = GetInteger ();
		}
		else if ( IsEqual ( szToken, ID_TICKSPERFRAME ) )
		{
			pScene->ticksPerFrame = GetInteger ();
		}
	} while ( nLevel > 0 );
}
