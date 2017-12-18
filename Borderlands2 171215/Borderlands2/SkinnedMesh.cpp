#include "stdafx.h"
#include "AllocateHierarchy.h"
#include "MatrialTexture.h"
#include "SkinnedMesh.h"

SkinnedMesh::SkinnedMesh()
	: root(nullptr)
	, animationController(nullptr)
	//, blendTime (0.3f)
	//, passedBlendTime(0.0f)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)

{
}

SkinnedMesh::SkinnedMesh(char* szFolder, char* szFilename)
	: root(NULL)
	, animationController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)
{
	SkinnedMesh* skinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

	root = skinnedMesh->root;
	m_dwWorkingPaletteSize = skinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = skinnedMesh->m_pmWorkingPalette;
	m_pEffect = skinnedMesh->m_pEffect;
	m_stBoundingSphere = skinnedMesh->m_stBoundingSphere;

	skinnedMesh->animationController->CloneAnimationController(
		skinnedMesh->animationController->GetMaxNumAnimationOutputs(),
		skinnedMesh->animationController->GetMaxNumAnimationSets(),
		skinnedMesh->animationController->GetMaxNumTracks(),
		skinnedMesh->animationController->GetMaxNumEvents(),
		&animationController);
}



SkinnedMesh::~SkinnedMesh()
{
	//AllocateHierarchy alloc;
	//D3DXFrameDestroy ( root, &alloc );

	SAFE_RELEASE(animationController);
}

void SkinnedMesh::Load(const char * foldername, const char * filename)
{
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	std::string folder(foldername);
	std::string fullpath = foldername + std::string("\\") + filename;

	AllocateHierarchy alloc;
	alloc.SetFolder(folder);
	alloc.SetDefaultPaletteSize(nPaletteSize);

	m_stBoundingSphere.vCenter = (alloc.GetMinVector() + alloc.GetMaxVector()) / 2.0f;
	m_stBoundingSphere.fRadius = D3DXVec3Length(&(alloc.GetMinVector() - alloc.GetMaxVector()));

	D3DXLoadMeshHierarchyFromXA(
		fullpath.c_str(),
		D3DXMESH_MANAGED,
		D3DDEVICE,
		&alloc,
		nullptr,
		(LPD3DXFRAME*)&root,
		&animationController
	);

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = alloc.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (root) SetupBoneMatrixPtrs(root);
}

void SkinnedMesh::Update(Bone* frame, D3DXMATRIX* pmatParent)
{
	frame->CombinedTransformationMatrix = frame->TransformationMatrix;

	if (pmatParent)
	{
		frame->CombinedTransformationMatrix = frame->CombinedTransformationMatrix * (*pmatParent);
	}

	if (frame->pFrameSibling)
	{
		Update((Bone*)frame->pFrameSibling, pmatParent);
	}

	if (frame->pFrameFirstChild)
	{
		Update((Bone*)frame->pFrameFirstChild, &(frame->CombinedTransformationMatrix));
	}
}

D3DXVECTOR3 SkinnedMesh::GetCamBonePos()
{
	Bone* camBone = (Bone*)D3DXFrameFind(root, "Camera");
	return D3DXVECTOR3(camBone->CombinedTransformationMatrix._41 + 0.15f, 
					   camBone->CombinedTransformationMatrix._42, 
					   camBone->CombinedTransformationMatrix._43);
}

void SkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME frame)
{
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
	// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
	// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
	// 찾아서 월드매트릭스를 걸어줘라.
	if (frame == nullptr)
		return;
	Bone* bone = (Bone*)frame;
	if (bone->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		if (boneMesh->pSkinInfo)
		{
			LPD3DXSKININFO skinInfo = boneMesh->pSkinInfo;
			DWORD dwNumBones = boneMesh->pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = skinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;

				Bone* p = (Bone*)D3DXFrameFind(root, skinInfo->GetBoneName(i));
				boneMesh->ppBoneMatrixPtrs[i] = &(p->CombinedTransformationMatrix);
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (frame->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(frame->pFrameFirstChild);
	}

	if (frame->pFrameSibling)
	{
		SetupBoneMatrixPtrs(frame->pFrameSibling);
	}

}

void SkinnedMesh::Moving()
{
	D3DXMatrixTranslation(&worldMat, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMat);
}

void SkinnedMesh::UpdateAndRender()
{
	if (animationController)
	{
		animationController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	}

	if (root)
	{
		Moving();
		Update(root, &worldMat);
		Render(root);
	}
}

void SkinnedMesh::Render(Bone* pBone)
{
	assert(pBone);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 ppBoneMatrixPtrs에 연결한다.
	if (pBone->pMeshContainer)
	{
		BoneMesh* pBoneMesh = (BoneMesh*)pBone->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		D3DDEVICE->GetTransform(D3DTS_VIEW, &matView);
		D3DDEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;
		
		D3DXMATRIXA16 mView, mInvView;
		D3DDEVICE->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			m_pEffect->SetMatrixArray("amPalette",
									  m_pmWorkingPalette,
									  pBoneMesh->dwNumPaletteEntries);

			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);

			// set the technique we use to draw
			m_pEffect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			m_pEffect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				m_pEffect->BeginPass(uiPass);
				pBoneMesh->workMesh->DrawSubset(dwAttrib);
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (pBone->pFrameSibling)
	{
		Render((Bone*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		Render((Bone*)pBone->pFrameFirstChild);
	}
}


void SkinnedMesh::SetAnimationIndex(int index)
{
	if (!animationController)
		return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	animationController->GetAnimationSet(index, &pAnimSet);
	animationController->SetTrackAnimationSet(0, pAnimSet);
	SAFE_RELEASE(pAnimSet);

}

void SkinnedMesh::Destroy()
{
	AllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)root, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}


void SkinnedMesh::SetRandomTrackPosition()
{
	animationController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}


LPD3DXEFFECT SkinnedMesh::LoadEffect(char* szFilename)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	D3DDEVICE->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	HRESULT hr;
	if (FAILED(hr = D3DXCreateEffectFromFileA(D3DDEVICE,
											  szFilename,
											  pmac,
											  NULL,
											  dwShaderFlags,
											  NULL,
											  &pEffect,
											  &pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}
