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
	, isMove(true)
{
}

SkinnedMesh::SkinnedMesh(char* szFolder, char* szFilename)
	: root(NULL)
	, animationController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vPosition(0, 0, 0)
	, isMove(true)
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

	D3DXLoadMeshHierarchyFromXA(
		fullpath.c_str(),
		D3DXMESH_MANAGED,
		D3DDEVICE,
		&alloc,
		nullptr,
		(LPD3DXFRAME*)&root,
		&animationController
	);

	m_stBoundingSphere.vCenter = (alloc.GetMinVector() + alloc.GetMaxVector()) / 2.0f;
	m_stBoundingSphere.fRadius = D3DXVec3Length(&(alloc.GetMinVector() - alloc.GetMaxVector()));
	D3DXCreateSphere(D3DDEVICE, m_stBoundingSphere.fRadius, 25, 25, &mesh, nullptr);
	
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

void SkinnedMesh::Blending()
{
	float deltaTime = g_pTimeManager->GetDeltaTime();
	animationController->AdvanceTime(deltaTime, nullptr);

	if (passedBlendTime <= blendTime) {
		passedBlendTime += deltaTime;

		if (passedBlendTime < blendTime) {
			float weight = passedBlendTime / blendTime;
			animationController->SetTrackWeight(0, weight);
			animationController->SetTrackWeight(1, 1.0f - weight);
		}
		else
		{
			animationController->SetTrackWeight(0, 1);
			animationController->SetTrackWeight(1, 0);
			animationController->SetTrackEnable(1, false);
		}
	}
}

D3DXVECTOR3 SkinnedMesh::GetCamBonePos(LPCSTR str)
{
	Bone* camBone = (Bone*)D3DXFrameFind(root, str);
	if (camBone) return D3DXVECTOR3(camBone->CombinedTransformationMatrix._41, 
					   camBone->CombinedTransformationMatrix._42, 
					   camBone->CombinedTransformationMatrix._43);
}

D3DXMATRIX SkinnedMesh::GetCamBoneMag05()
{
	Bone* MagBone = (Bone*)D3DXFrameFind(root, "Mag05");
	return MagBone->CombinedTransformationMatrix;
}

bool SkinnedMesh::GetAnimEnd(UINT animIndex)
{
	LPD3DXANIMATIONSET pAnimSet;
	animationController->GetAnimationSet(animIndex, &pAnimSet);
	float dPeriod = pAnimSet->GetPeriod();

	D3DXTRACK_DESC tc;
	animationController->GetTrackDesc(0, &tc);
	float dPosition = tc.Position;

	if (dPeriod - 0.45f > dPosition)
	{
		return false;
	}

	return true;
}

bool SkinnedMesh::GetAnimHalf(UINT animIndex)
{
	LPD3DXANIMATIONSET pAnimSet;
	animationController->GetAnimationSet(animIndex, &pAnimSet);
	float dPeriod = pAnimSet->GetPeriod();

	D3DXTRACK_DESC TrackDesc_0;
	animationController->GetTrackDesc(0, &TrackDesc_0);
	float dPosition = TrackDesc_0.Position;

	float fHalf = (dPosition / dPeriod);

	if (fHalf > 0.5f && fHalf < 1.f)
	{
		return true;
	}
	else if (fHalf > 1.f)
	{
		dPosition = 0;
		return false;
	}

	return false;
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
	// 지영 수정==============================================
	// 다른 SkinnedMesh까지 같이 움직이지 않게 수정
	// 변경하지마 변경하지마 변경하지마 변경하지마 변경하지마
	float tempAngleX, tempAngleY;
	D3DXMATRIX rotationX, rotationY, rotation, translation;

	// isMove = true 오직 플레이어만 
	if (isMove) {
		tempAngleX = -g_pCamera->GetAngleX();
		tempAngleY = g_pCamera->GetAngleY() - 1.5f;
	}
	else { // 그 외
		tempAngleX = angleX;
		tempAngleY = angleY;
	}

	D3DXMatrixIdentity(&rotationX);
	D3DXMatrixRotationZ(&rotationX, tempAngleX);
	D3DXMatrixRotationY(&rotationY, tempAngleY);
	D3DXMatrixTranslation(&translation, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	if (isMove) {	// 플레이어만 
		rotation = rotationX * rotationY;
		worldMat = rotation * translation;
	}
	else {
		worldMat = rotationY * translation;
	}
	// =======================================================
	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMat);

	LPD3DXFONT font = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc = { 0, 0, 200, 200 };
	char str[1024];
	sprintf(str, "%.5f, %.5f", g_pCamera->GetAngleX(), g_pCamera->GetAngleY());
	font->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 0, 0));
}

void SkinnedMesh::UpdateAndRender()
{
	if (animationController)
	{
		animationController->AdvanceTime(g_pTimeManager->GetDeltaTime(), NULL);
	}

	if (root)
	{
		D3DXMATRIX matS, matR, matT;
		D3DXMatrixScaling(&matS, scale, scale, scale);
		D3DXMatrixRotationX(&matR, rotX);
		D3DXMatrixRotationY(&matR, rotY);
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		worldMat = matS * matR * matT;
		D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMat);
		Moving();
		Blending();
		DrawSphere();
		Update(root, &worldMat);
		Render(root);
	}
}

void SkinnedMesh::DrawSphere()
{
	if (mesh)
	{
		D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);
		D3DMATERIAL9 material;
		ZeroMemory(&material, sizeof D3DMATERIAL9);
		material.Ambient = material.Diffuse = material.Specular = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		D3DDEVICE->SetMaterial(&material);
		mesh->DrawSubset(0);
		D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
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


void SkinnedMesh::SetAnimationIndex(int index, bool isBlend)
{
	if (!animationController) return;
	LPD3DXANIMATIONSET pNextAniSet = nullptr;
	animationController->GetAnimationSet(index, &pNextAniSet);

	if (isBlend) {
		LPD3DXANIMATIONSET pPrevAniSet = nullptr;
		animationController->GetTrackAnimationSet(0, &pPrevAniSet);
		animationController->SetTrackAnimationSet(1, pPrevAniSet);

		D3DXTRACK_DESC trackDesc;
		animationController->GetTrackDesc(0, &trackDesc);
		animationController->SetTrackDesc(1, &trackDesc);

		animationController->SetTrackWeight(0, 0.0f);
		animationController->SetTrackWeight(1, 1.0f);
		SAFE_RELEASE(pPrevAniSet);

		passedBlendTime = 0.0f;
	}

	animationController->SetTrackAnimationSet(0, pNextAniSet);
	SAFE_RELEASE(pNextAniSet);
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