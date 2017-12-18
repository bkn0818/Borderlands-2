#include "stdafx.h"
#include "MatrialTexture.h"
#include "AllocateHierarchy.h"


AllocateHierarchy::AllocateHierarchy ()
	: folder ( "" )
	, defaultPaletteSize ( 0 )
	, maxPaletteSize ( 0 )
	, minVector ( 0, 0, 0 )
	, maxVector ( 0, 0, 0 )
{
}


AllocateHierarchy::~AllocateHierarchy ()
{
}

STDMETHODIMP AllocateHierarchy::CreateFrame ( LPCSTR Name, LPD3DXFRAME * ppNewFrame )
{
	Bone* frame = new Bone;

	frame->Name = NULL;
	if ( Name )
	{
		frame->Name = new char[strlen ( Name ) + 1];
		strcpy ( frame->Name, Name );
	}

	frame->pFrameFirstChild = nullptr;
	frame->pFrameSibling = nullptr;
	frame->pMeshContainer = nullptr;

	D3DXMatrixIdentity ( &frame->TransformationMatrix );
	D3DXMatrixIdentity ( &frame->CombinedTransformationMatrix );

	*ppNewFrame = frame;

	return S_OK;

}

STDMETHODIMP AllocateHierarchy::CreateMeshContainer ( 
	LPCSTR Name, 
	CONST D3DXMESHDATA * pMeshData, 
	CONST D3DXMATERIAL * pMaterials, 
	CONST D3DXEFFECTINSTANCE * pEffectInstances, 
	DWORD NumMaterials, 
	CONST DWORD * pAdjacency, 
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER * ppNewMeshContainer )
{

	assert ( pMeshData && pMeshData->Type == D3DXMESHTYPE_MESH );

	// 메쉬 컨테이너 생성
	BoneMesh* pBoneMesh = new BoneMesh;

	// 이름 무시
	pBoneMesh->Name = NULL;


	for ( DWORD i = 0; i < NumMaterials; ++i )
	{
		// 재질정보 저장
		pBoneMesh->vecMaterial.push_back ( pMaterials[i].MatD3D );
		// 텍스쳐 정보 저장.
		std::string sFilename ( pMaterials[i].pTextureFilename );
		std::string sFullPath = folder + sFilename;
		LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture ( sFullPath );
		pBoneMesh->vecTexture.push_back ( pTex );
	}

	// 이펙트 무시
	pBoneMesh->pEffects = nullptr;

	// 인접정보 복사
	if ( pAdjacency )
	{
		DWORD dwNumFaces = pMeshData->pMesh->GetNumFaces ();
		pBoneMesh->pAdjacency = new DWORD[3 * dwNumFaces];
		memcpy ( pBoneMesh->pAdjacency, pAdjacency, 3 * sizeof ( DWORD ) * dwNumFaces );
	}


	// step 1. pSkinInfo 저장
	SAFE_ADDREF ( pSkinInfo );
	pBoneMesh->pSkinInfo = pSkinInfo;

	// step 2. 메쉬 복사
	//SAFE_ADDREF ( pMeshData->pMesh );
	//pBoneMesh->orgMesh = pMeshData->pMesh;
	//pBoneMesh->orgMesh->CloneMeshFVF (
	//	pBoneMesh->orgMesh->GetOptions (),
	//	pBoneMesh->orgMesh->GetFVF (),
	//	D3DDEVICE,
	//	&pBoneMesh->workMesh );

	if ( pMeshData && pMeshData->pMesh )
	{
		D3DXVECTOR3 vMin ( 0, 0, 0 ), vMax ( 0, 0, 0 );
		LPVOID pV = NULL;

		pMeshData->pMesh->LockVertexBuffer ( 0, &pV );
		D3DXComputeBoundingBox ( ( D3DXVECTOR3* ) pV,
			pMeshData->pMesh->GetNumVertices (),
			D3DXGetFVFVertexSize ( pMeshData->pMesh->GetFVF () ),
			&vMin,
			&vMax );
		D3DXVec3Minimize ( &minVector, &minVector, &vMin );
		D3DXVec3Maximize ( &maxVector, &maxVector, &vMax );
		pMeshData->pMesh->UnlockVertexBuffer ();

		pMeshData->pMesh->CloneMeshFVF (
			pMeshData->pMesh->GetOptions (),
			pMeshData->pMesh->GetFVF (),
			D3DDEVICE,
			&pBoneMesh->orgMesh );
	}




	// step 3. pSkinInfo->GetNumBones()를 통해
	// 영향력을 미치는 모든 본에 대한 매트릭스 들을 세팅
	// ppBoneMatrixPtrs, pBoneOffsetMatrices, pCurrentBoneMatrices를 동적할당
	//DWORD dwNumBones = pSkinInfo->GetNumBones ();
	//pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];
	//pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
	//pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBones];

	//// step 4. 동적 할당된 pBoneOffsetMatrices 매트릭스에 값 저장.
	//// pSkinInfo->GetBoneOffsetMatrix(i)
	//for ( DWORD i = 0; i < dwNumBones; ++i )
	//{
	//	pBoneMesh->pBoneOffsetMatrices[i] = *( pSkinInfo->GetBoneOffsetMatrix ( i ) );
	//}
	//pBoneMesh->pEffects = NULL;
	//pBoneMesh->pAdjacency = NULL;

	if ( pSkinInfo ) {
		// pSkinInfo->GetNumBones()를 통해 영향력을 미치는 모든 본에 대한 매트릭스 들을 세팅
		// ppBoneMatrixPtrs, pBoneOffsetMatrices를 동적할당
		DWORD dwNumBones = pSkinInfo->GetNumBones ();
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
		for ( DWORD i = 0; i < dwNumBones; ++i )
		{
			pBoneMesh->ppBoneMatrixPtrs[i] = NULL;
			pBoneMesh->pBoneOffsetMatrices[i] = *( pSkinInfo->GetBoneOffsetMatrix ( i ) );
		}

		// ConvertToIndexedBlendedMesh 함수에 팔렛트 엔트리 개수를 넘겨야하기 때문에 
		// 멤버변수로 세팅하고 외부에서 적용해주도록 설정.
		// 메쉬에 영향을 주는 팔레트의 개수는 
		// m_dwDefaultPaletteSize와 pSkinInfo->GetNumBones()보다 클 수 없다.
		// m_dwDefaultPaletteSize는 쉐이더에 정의되어있다.
		pBoneMesh->dwNumPaletteEntries = min ( defaultPaletteSize, pSkinInfo->GetNumBones () );

		// 최대 팔레트 사이즈 업데이트
		if ( maxPaletteSize < pBoneMesh->dwNumPaletteEntries )
		{
			maxPaletteSize = pBoneMesh->dwNumPaletteEntries;
		}

		// blend weights와 인덱스를 이용해 workMesh 생성.
		pBoneMesh->pSkinInfo->ConvertToIndexedBlendedMesh (
			pBoneMesh->orgMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
			pBoneMesh->dwNumPaletteEntries,
			pBoneMesh->pAdjacency,
			NULL,
			NULL,
			NULL,
			&pBoneMesh->dwMaxNumFaceInfls,
			&pBoneMesh->dwNumAttrGroups,
			&pBoneMesh->pBufBoneCombos,
			&pBoneMesh->workMesh );

		// 기존 메쉬의 FVF가 버텍스 블랜딩 정보를 전달 할 수 없는 경우 FVF를 변경한다
		DWORD dwOldFVF = pBoneMesh->workMesh->GetFVF ();
		DWORD dwNewFVF =
			( dwOldFVF & D3DFVF_POSITION_MASK ) |
			D3DFVF_NORMAL |
			D3DFVF_TEX1 |
			D3DFVF_LASTBETA_UBYTE4;

		if ( dwNewFVF != dwOldFVF )
		{
			// 메쉬를 복사하면서 FVF를 변경한다.
			LPD3DXMESH pMesh = NULL;

			pBoneMesh->workMesh->CloneMeshFVF (
				pBoneMesh->workMesh->GetOptions (),
				dwNewFVF,
				D3DDEVICE,
				&pMesh );

			// 기존 메쉬를 대체
			SAFE_RELEASE ( pBoneMesh->workMesh );
			pBoneMesh->workMesh = pMesh;

			// 새로 생성한 메쉬에는 노멀 정보가 있으므로
			// 기존 메쉬 FVF에 노멀 정보가 없으면 새로 계산해줘야함.
			if ( !( dwOldFVF & D3DFVF_NORMAL ) )
			{
				D3DXComputeNormals ( pBoneMesh->workMesh, NULL );
			}
		}

		// Interpret the UBYTE4 as a D3DCOLOR.
		// The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
		// blend indices to a D3DCOLOR semantic, and later in the shader convert
		// it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
		// convert any data, just the declaration.
		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		D3DVERTEXELEMENT9 * pDeclCur;
		pBoneMesh->workMesh->GetDeclaration ( pDecl );

		pDeclCur = pDecl;
		while ( pDeclCur->Stream != 0xff )
		{
			if ( ( pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES ) &&
				( pDeclCur->UsageIndex == 0 ) )
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}
		pBoneMesh->workMesh->UpdateSemantics ( pDecl );
	}

	*ppNewMeshContainer = pBoneMesh;

	return S_OK;

}

STDMETHODIMP AllocateHierarchy::DestroyFrame ( LPD3DXFRAME pFrameToFree )
{
	// TODO : 해제 잘 합시다.
	Bone* bone = ( Bone* ) pFrameToFree;

	SAFE_DELETE_ARRAY ( bone->Name );
	SAFE_DELETE ( bone );

	return S_OK;

}

STDMETHODIMP AllocateHierarchy::DestroyMeshContainer ( LPD3DXMESHCONTAINER pMeshContainerToFree )
{
	// TODO : 해제 잘 합시다.

	BoneMesh* boneMesh = ( BoneMesh* ) pMeshContainerToFree;

	SAFE_RELEASE ( boneMesh->pSkinInfo );
	SAFE_RELEASE ( boneMesh->workMesh );
	SAFE_RELEASE ( boneMesh->orgMesh );
	SAFE_RELEASE ( boneMesh->pBufBoneCombos);

	SAFE_DELETE_ARRAY ( boneMesh->Name );
	SAFE_DELETE_ARRAY ( boneMesh->pAdjacency );
	SAFE_DELETE_ARRAY ( boneMesh->ppBoneMatrixPtrs );
	SAFE_DELETE_ARRAY ( boneMesh->pBoneOffsetMatrices );

	SAFE_DELETE ( pMeshContainerToFree );
	return S_OK;

}
