#include "stdafx.h"
#include "Plane.h"

Plane::Plane()
{
	m_ixDIB = 0;
	m_izDIB = 0;
	m_iyDIB = 0;

	m_iWidth = 0;
	m_iDepth = 0;
	m_fTexScale = 10.0f;

	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pHeightMap = nullptr;
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		m_pTex[i] = nullptr;
	}
	memset(m_pAlpha, 0, sizeof(LPDIRECT3DTEXTURE9) * MAX_TEXTURE);

	m_iSplateIdx = 0;
	m_iBaseTex = 0;
	m_pVB = nullptr;
	m_pIB = nullptr;
	m_pVBPicking = nullptr;
	m_iTriangles = 0;
	m_pQuadTree = nullptr;
	iBigCircle = 20;
	iSmallCircle = 10;
	m_iTexPosX = 0;
	m_iTexPosY = 0;
	m_iPickingvalue = 1;
	m_iUpdown = 1;
	m_iBigValue = 1;
	m_iSmallValue = 1;

	memset(m_iSplateTex, -1, sizeof(int) * MAX_TEXTURE);
}


Plane::~Plane()
{
	Destroy();
}

HRESULT Plane::InitPlane(D3DXVECTOR3 * pScale, float fLODRatio, char * lpBMPFileName, LPSTR lpTexFileName)
{
	m_vScale = *pScale;
	m_fLODRatio = fLODRatio;
	if (FAILED(BuildHeightMap(lpBMPFileName)))
	{
		Destroy();
		return E_FAIL;
	}
	if (FAILED(LoadTexture(lpTexFileName)))
	{
		Destroy();

		return E_FAIL;
	}

	if (FAILED(InitVIB()))
	{
		Destroy();

		return E_FAIL;
	}

	if (FAILED(CreateAlpha()))
	{
		return E_FAIL;
	}

	m_pQuadTree = new QuadTree(m_ixDIB, m_izDIB);

	if (FAILED(BuildQuadTree()))
	{
		Destroy();

		return E_FAIL;
	}

	m_iWidth = (int)(m_ixDIB * m_vScale.x);
	m_iDepth = (int)(m_izDIB * m_vScale.z);

	m_iSplateTex[0] = 0;
	m_iSplateTex[1] = 1;

	return S_OK;
}

HRESULT Plane::Render()
{
	//0번 스테이지에 텍스처 고정(색깔맵)
	//D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DDEVICE->SetTexture(0, m_pTex[0]);
	D3DDEVICE->SetStreamSource(0, m_pVB, 0, sizeof(VertexPNT));
	D3DDEVICE->SetFVF(VertexPNT::FVF);
	D3DDEVICE->SetIndices(m_pIB);
	D3DXMATRIX mat;
	D3DXMatrixScaling(&mat, m_fTexScale, m_fTexScale, 1.0f);
	D3DDEVICE->SetTransform(D3DTS_TEXTURE0, &mat);

	D3DDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_ixDIB * m_izDIB, 0, m_iTriangles);

	return S_OK;
}

HRESULT Plane::RenderCircle(Picking * pRay)
{
	//피킹원 정점버퍼 생성
	D3DDEVICE->SetStreamSource(0, m_pVBPicking, 0, sizeof(CUSTOMCIRCLE));
	D3DDEVICE->SetFVF(CUSTOMCIRCLE::FVF);

	//텍스처 제거
	D3DDEVICE->SetTexture(0, nullptr);

	//큰원 색 설정
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Emissive = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	D3DDEVICE->SetMaterial(&material);
	//큰원 그리기
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 0, 180);
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 1, 179);

	//작은원 색 설정
	material.Emissive = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
	D3DDEVICE->SetMaterial(&material);
	//작은원 그리기
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 360, 180);
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 361, 179);

	return S_OK;
}

HRESULT Plane::Destroy()
{
	if (m_pQuadTree) {
		SAFE_DELETE_ARRAY(m_pHeightMap);
		m_pHeightMap = nullptr;
		SAFE_DELETE(m_pQuadTree);
		m_pQuadTree = nullptr;
	}
	if (m_pIB) {
		SAFE_RELEASE(m_pIB);
		m_pIB = nullptr;
	}
	if (m_pVB) {
		SAFE_RELEASE(m_pVB);
		m_pVB = nullptr;
	}
	if (m_pVBPicking) {
		SAFE_RELEASE(m_pVBPicking);
		m_pVBPicking = nullptr;
	}
	for (int i = 0; i < MAX_TEXTURE; ++i)
	{
		if (m_pTex[i]) {
			SAFE_RELEASE(m_pTex[i]);
			m_pTex[i] = nullptr;
		}
		if (m_pAlpha[i]) {
			SAFE_RELEASE(m_pAlpha[i]);
			m_pAlpha[i] = nullptr;
		}
	}

	return S_OK;
}

HRESULT Plane::LoadTexture(LPSTR lpTexFilename)
{
	m_pTex[0] = g_pTextureManager->GetTexture(lpTexFilename);
	m_pTex[1] = g_pTextureManager->GetTexture(".\\Texture\\Texture\\황색가뭄.bmp");

	return S_OK;
}

//BMP파일로 높이맵 생성
HRESULT Plane::BuildHeightMap(char * lpFilename)
{
	LPBYTE lpDIB = DibLoadHandle(lpFilename);
	int n;

	if (!lpDIB) return E_FAIL;

	m_ixDIB = ((LPBITMAPINFOHEADER)lpDIB)->biWidth;
	m_izDIB = ((LPBITMAPINFOHEADER)lpDIB)->biHeight;

	//ixDIB, izDIB가 (2^n + 1)이 아닌 경우 실패 (2의 배수만 생성가능(쿼드트리))
	n = Log2(m_ixDIB);
	if ((Pow2(n) + 1) != m_ixDIB) return E_FAIL;
	n = Log2(m_izDIB);
	if ((Pow2(n) + 1) != m_izDIB) return E_FAIL;

	m_pHeightMap = new VertexPNT[m_ixDIB * m_izDIB];
	VertexPNT v;

	for (int z = 0; z < m_izDIB; ++z)
	{
		for (int x = 0; x < m_ixDIB; ++x)
		{
			v.p.x = (float)((x - m_ixDIB / 2.0f) * m_vScale.x);
			v.p.z = -(float)((z - m_izDIB / 2.0f) * m_vScale.z);
			v.p.y = (float)(*(DIBXYInv(lpDIB, x, z))) * m_vScale.y;

			D3DXVec3Normalize(&v.n, &v.p);

			v.t.x = (float)x / (float)(m_ixDIB - 1);
			v.t.y = (float)z / (float)(m_izDIB - 1);

			m_pHeightMap[x + z * m_izDIB] = v;
		}
	}
	DibDeleteHandle(lpDIB);

	return S_OK;
}

HRESULT Plane::BuildQuadTree()
{
	m_pQuadTree->Build(m_pHeightMap);

	return S_OK;
}

HRESULT Plane::InitVIB()
{
	//정점버퍼 생성
	if (FAILED(D3DDEVICE->CreateVertexBuffer(m_ixDIB * m_izDIB * sizeof(VertexPNT), 0, VertexPNT::FVF, D3DPOOL_DEFAULT, &m_pVB, nullptr)))
	{
		Destroy();
		return E_FAIL;
	}

	void* pVertices;
	if (FAILED(m_pVB->Lock(0, m_ixDIB * m_izDIB * sizeof(VertexPNT), (LPVOID*)&pVertices, 0)))
	{
		Destroy();
		return E_FAIL;
	}
	memcpy(pVertices, m_pHeightMap, m_ixDIB * m_izDIB * sizeof(VertexPNT));
	m_pVB->Unlock();

	//인덱스버퍼 생성
	if (FAILED(D3DDEVICE->CreateIndexBuffer((m_ixDIB - 1) * (m_izDIB - 1) * 2 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, nullptr)))
	{
		Destroy();
		return E_FAIL;
	}
	if (!m_pVBPicking)
	{
		if (FAILED(D3DDEVICE->CreateVertexBuffer(720 * sizeof(CUSTOMCIRCLE), 0, CUSTOMCIRCLE::FVF, D3DPOOL_DEFAULT, &m_pVBPicking, nullptr)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT Plane::DrawMesh(FrustumCulling * pFrustum, Picking * pRay, bool sFlag)
{
	LPWORD pI;

	if (FAILED(m_pIB->Lock(0, (m_ixDIB - 1) * (m_izDIB - 1) * 2 * sizeof(MYINDEX), (LPVOID*)&pI, 0))) return E_FAIL;

	m_iTriangles = m_pQuadTree->GetIndex(pI, m_pHeightMap, pFrustum, m_fLODRatio);
	m_pIB->Unlock();

	if (sFlag) DrawAlpha(pRay, m_iSplateTex[m_iSplateIdx]);

	D3DDEVICE->SetStreamSource(0, m_pVB, 0, sizeof(VertexPNT));
	D3DDEVICE->SetFVF(VertexPNT::FVF);
	D3DDEVICE->SetIndices(m_pIB);
	D3DXMATRIX mat;
	SetBaseTexture();
	D3DXMatrixScaling(&mat, m_fTexScale, m_fTexScale, 1.0f);
	D3DDEVICE->SetTransform(D3DTS_TEXTURE0, &mat);

	D3DDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_ixDIB * m_izDIB, 0, m_iTriangles);

	//0번 텍스처의 좌표를 래스터라이저에게서 직접 얻음
	D3DDEVICE->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	//알파블렌드 사용
	D3DDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	RenderSplatting();
	RenderCircle(pRay);

	return S_OK;
}

DWORD Plane::ReturnZH()
{
	return m_izDIB * m_vScale.z;
}

int Plane::Log2(int n)
{
	for (int i = 1; i < 64; i++)
	{
		n = n >> 1;

		if (n == 1)
		{
			return i;
		}
	}

	return 1;
}

int Plane::Pow2(int n)
{
	int val = 1;
	val = val << n;
	return val;
}

LPBYTE Plane::DIBXYInv(LPBYTE lpDIB, int x, int z)
{
	return (lpDIB + sizeof(BITMAPINFOHEADER) + PalSize(lpDIB)) +
		x * ((LPBITMAPINFOHEADER)lpDIB)->biBitCount / 8 +
		(((LPBITMAPINFOHEADER)lpDIB)->biHeight - z - 1) *
		ALIGN4B(((LPBITMAPINFOHEADER)lpDIB)->biWidth *
		((LPBITMAPINFOHEADER)lpDIB)->biBitCount / 8);
}

int Plane::PalSize(LPBYTE lpDIB)
{
	if (((LPBITMAPINFOHEADER)lpDIB)->biBitCount == 24)
	{
		return 1;
	}
	else
	{
		return sizeof(RGBQUAD) * (1 << ((LPBITMAPINFOHEADER)lpDIB)->biBitCount);
	}
}

int Plane::ALIGN4B(int n)
{
	if (n % 4)
	{
		return n + 4 - (n % 4);
	}
	else
	{
		return n;
	}
}

LPBYTE Plane::DibLoadHandle(char * lpFileName)
{
	FILE* fp;
	BITMAPFILEHEADER bmfh;
	HGLOBAL hDib;
	LPBYTE	lpDib;

	int iSize;
	if (!(fp = fopen(lpFileName, "rb"))) return nullptr;

	fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);

	iSize = bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	hDib = (HGLOBAL)GlobalAlloc(GMEM_FIXED, iSize);
	lpDib = (LPBYTE)GlobalLock(hDib);

	if (!lpDib) {
		fclose(fp);
		return nullptr;
	}

	fread(lpDib, iSize, 1, fp);
	fclose(fp);
	return lpDib;
}

void Plane::DibDeleteHandle(LPBYTE lpSrc)
{
	if (lpSrc) {
		GlobalUnlock(lpSrc);
		GlobalFree(lpSrc);
	}
}

//지형 피킹
bool Plane::SearchPicking(Picking * pRay, MYINDEX * pIdx)
{
	bool bResult = false;

	//피킹 검색(T값이 가장 작은 삼각형 선택)
	D3DXVECTOR3 vPick;
	float fMin = (float)m_iDepth * 2;
	for (int i = 0; i < m_iTriangles; ++i) {
		//가장 가까운 피킹포인트 구하기
		if (pRay->RayTriangleIntTest(m_pHeightMap[pIdx[i].a].p,
									 m_pHeightMap[pIdx[i].b].p, m_pHeightMap[pIdx[i].c].p))
		{
			if (pRay->ray.fT < fMin)
			{
				bResult = true;
				fMin = pRay->ray.fT;
				vPick = pRay->PickPos;
			}
		}
	}

	pRay->PickPos = vPick;
	pRay->PickPos.y = GetHeightMapY(pRay->PickPos.x, pRay->PickPos.z);

	return bResult;
}

D3DXVECTOR3 Plane::GetHeightMap(int row, int col)
{
	if (0 > row || row < m_ixDIB - 1) return D3DXVECTOR3(0, 0, 0);
	if (0 > col || col > m_izDIB - 1) return D3DXVECTOR3(0, 0, 0);
	return m_pHeightMap[col * m_ixDIB + row].p;
}

float Plane::GetHeightMapY(float x, float z)
{
	//x, z값의 원점을 지형의 원점으로 이동
	x = m_iWidth / 2.0f + x;
	z = m_iDepth / 2.0f - z;

	//각 간격을 나눠 셀 간격을 1로 변환(인덱스 찾기용)
	x /= m_vScale.x;
	z /= m_vScale.z;

	int iRow = (int)floorf(x);
	int iCol = (int)floorf(z);

	D3DXVECTOR3 temp;
	float a, b, c, d;
	//셀 설정
	temp = GetHeightMap(iRow, iCol);
	a = temp.y;
	temp = GetHeightMap(iRow + 1, iCol);
	b = temp.y;
	temp = GetHeightMap(iRow, iCol + 1);
	c = temp.y;
	temp = GetHeightMap(iRow + 1, iCol + 1);
	d = temp.y;

	//보간율 계산
	float fdx = x - iRow;
	float fdz = z - iCol;

	//높이별 선형보간
	float fHeight, fuy, fvy;
	if (fdz < 1.0f - fdx)
	{
		fuy = b - a;
		fvy = c - a;
		fHeight = a + LinearInterpolation(0.0f, fuy, fdx) + LinearInterpolation(0.0f, fvy, fdz);
	}
	else
	{
		fuy = c - d;
		fvy = b - d;
		fHeight = d + LinearInterpolation(0.0f, fuy, 1.0f - fdx) + LinearInterpolation(0.0f, fvy, 1.0f - fdz);
	}
	return fHeight;
}

void Plane::SetHeightMapY(int row, int col, int y)
{
	if (0 > row || row > m_ixDIB - 1)
	{
		return;
	}

	if (0 > col || col > m_izDIB - 1)
	{
		return;
	}

	if (y == 0.0f)
	{
		m_pHeightMap[(col * m_ixDIB) + row].p.y = 0.0f;
	}
	else
	{
		m_pHeightMap[(col * m_ixDIB) + row].p.y += y;
	}
}

void Plane::GetHeightMapIdx(float x, float z, int & iRow, int & iCol)
{
	//x, z값의 원점을 지형의 원점으로!!
	x = m_iWidth / 2.0f + x;
	z = m_iDepth / 2.0f - z;

	//셀 간격을 1로 변환!!
	x /= m_vScale.x;
	z /= m_vScale.z;

	iRow = (int)floorf(x);
	iCol = (int)floorf(z);

	//위치 테스트 (위 폴리곤인지 아래폴리곤인지)
	float fdx = x - iRow;
	float fdz = z - iCol;

	//아래폴리곤으로 인덱스설정
	if (fdz >= 1.0f - fdx)
	{
		++iRow;
		++iCol;
	}
}

float Plane::LinearInterpolation(float a, float b, float T)
{
	return a - (a * T) + (b * T);
}

bool Plane::PickingMap(Picking * pRay, bool buttonFlag)
{
	bool pickingCheck = false;
	int iTemp, iSRow, iERow, iSCol, iECol;

	MYINDEX* pIdx;
	if (FAILED(m_pIB->Lock(0, m_iTriangles * sizeof(MYINDEX), (LPVOID*)&pIdx, 0))) return false;

	pickingCheck = SearchPicking(pRay, pIdx);
	if (pickingCheck && (buttonFlag))
	{
		//지형 높낮이 조절!!!
		//큰원 : 최상위부터 하위까지 피킹범위 찾기
		GetHeightMapIdx(pRay->PickPos.x - iBigCircle, pRay->PickPos.z, iSRow, iTemp);
		GetHeightMapIdx(pRay->PickPos.x - iBigCircle, pRay->PickPos.z, iERow, iTemp);
		GetHeightMapIdx(pRay->PickPos.x, pRay->PickPos.z + iBigCircle, iTemp, iSCol);
		GetHeightMapIdx(pRay->PickPos.x, pRay->PickPos.z - iBigCircle, iTemp, iECol);

		//높이값 적용
		for (int iz = iSCol; iz <= iECol; ++iz) {
			for (int ix = iSRow; ix <= iERow; ++ix)
			{
				SetHeightMapY(ix, iz, m_iUpdown * m_iPickingvalue * m_iBigValue);
			}
		}
		//작은 원 : 최상위부터 하위까지 피킹범위 찾기
		GetHeightMapIdx(pRay->PickPos.x - iSmallCircle, pRay->PickPos.z, iSRow, iTemp);
		GetHeightMapIdx(pRay->PickPos.x + iSmallCircle, pRay->PickPos.z, iERow, iTemp);
		GetHeightMapIdx(pRay->PickPos.x, pRay->PickPos.z + iSmallCircle, iTemp, iSCol);
		GetHeightMapIdx(pRay->PickPos.x, pRay->PickPos.z - iSmallCircle, iTemp, iECol);

		//높이값 적용
		for (int iz = iSCol; iz <= iECol; ++iz)
		{
			for (int ix = iSRow; ix <= iERow; ++ix)
			{
				SetHeightMapY(ix, iz, 2 * m_iUpdown * m_iPickingvalue *
							  m_iSmallValue);
			}
		}
	}

	if (!MakeVBMap(pIdx)) return false;
	if (MakeVBPicking(pRay)) return E_FAIL;
	m_pIB->Unlock();
	return true;
}

bool Plane::MakeVBMap(MYINDEX * pIdx)
{
	//노멀 벡터 계산
	D3DXVECTOR3 a, b, c;

	for (int i = 0; i < m_iTriangles; ++i)
	{
		a = m_pHeightMap[pIdx[i].b].p - m_pHeightMap[pIdx[i].a].p;
		b = m_pHeightMap[pIdx[i].c].p - m_pHeightMap[pIdx[i].a].p;

		D3DXVec3Cross(&c, &a, &b);
		D3DXVec3Normalize(&c, &c);

		m_pHeightMap[pIdx[i].a].n += c;
		m_pHeightMap[pIdx[i].b].n += c;
		m_pHeightMap[pIdx[i].c].n += c;
	}

	//노멀벡터 정규화
	for (int i = 0; i < m_iTriangles; ++i)
	{
		D3DXVec3Normalize(&m_pHeightMap[pIdx[i].a].n, &m_pHeightMap[pIdx[i].a].n);
		D3DXVec3Normalize(&m_pHeightMap[pIdx[i].b].n, &m_pHeightMap[pIdx[i].b].n);
		D3DXVec3Normalize(&m_pHeightMap[pIdx[i].c].n, &m_pHeightMap[pIdx[i].c].n);
	}

	VertexPNT* pVertex;

	if (FAILED(m_pVB->Lock(0, m_ixDIB * m_izDIB * sizeof(VertexPNT),
		(void**)&pVertex, 0)))
	{
		return false;
	}

	memcpy(pVertex, m_pHeightMap, m_ixDIB * m_izDIB * sizeof(VertexPNT));

	m_pVB->Unlock();

	return true;
}

bool Plane::MakeVBPicking(Picking * pRay)
{
	CUSTOMCIRCLE *pCircle;

	if (FAILED(m_pVBPicking->Lock(0, 720 * sizeof(CUSTOMCIRCLE), (void**)&pCircle, 0)))
	{
		return false;
	}

	for (int i = 0; i < 360; ++i)
	{
		pCircle[i].Pos.x = iBigCircle * sin(i * 3.14f / 180.0f) + pRay->PickPos.x;
		pCircle[i].Pos.z = iBigCircle * cos(i * 3.14f / 180.0f) + pRay->PickPos.z;
		pCircle[i].Pos.y = GetHeightMapY(pCircle[i].Pos.x, pCircle[i].Pos.z) + 2.0f;
	}

	for (int i = 360; i < 720; ++i)
	{
		pCircle[i].Pos.x = iSmallCircle * sin((i - 360) * 3.14f / 180.0f) + pRay->PickPos.x;
		pCircle[i].Pos.z = iSmallCircle * cos((i - 360) * 3.14f / 180.0f) + pRay->PickPos.z;
		pCircle[i].Pos.y = GetHeightMapY(pCircle[i].Pos.x, pCircle[i].Pos.z) + 2.0f;
	}

	m_pVBPicking->Unlock();

	return true;
}

void Plane::SetBaseTexture()
{
	D3DDEVICE->SetTexture(0, m_pTex[0]);
	D3DDEVICE->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	D3DDEVICE->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	D3DDEVICE->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDEVICE->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	D3DDEVICE->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	D3DDEVICE->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS,
									D3DTTFF_COUNT2);
	D3DDEVICE->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	D3DDEVICE->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	D3DDEVICE->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

void Plane::SetAlphaTexture()
{
	//알파맵
	D3DDEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	D3DDEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	//위에 덮을 텍스쳐
	D3DDEVICE->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	D3DDEVICE->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	D3DDEVICE->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	D3DDEVICE->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);

	D3DDEVICE->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDEVICE->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	D3DDEVICE->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	D3DDEVICE->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS,
									D3DTTFF_DISABLE);
	D3DDEVICE->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS,
									D3DTTFF_COUNT2);
	D3DDEVICE->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

	D3DDEVICE->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	D3DDEVICE->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	D3DDEVICE->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	D3DDEVICE->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

void Plane::RenderSplatting()
{
	D3DXMATRIXA16 matTemp;

	//알파텍스쳐설정
	SetAlphaTexture();

	D3DXMatrixScaling(&matTemp, 20.0f, 20.0f, 1.0f);

	D3DDEVICE->SetTransform(D3DTS_TEXTURE0, &matTemp);
	D3DDEVICE->SetTransform(D3DTS_TEXTURE1, &matTemp);

	//알파텍스쳐 설정
	D3DDEVICE->SetTexture(0, m_pAlpha[1]);

	//일반텍스쳐 설정
	D3DDEVICE->SetTexture(1, m_pTex[1]);

	//출력
	D3DDEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
									m_ixDIB *	m_izDIB, 0, m_iTriangles);

	//1번 텍스쳐의 좌표를 레스터라이즈로부터 직접 받음
	D3DDEVICE->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS,
									D3DTTFF_DISABLE);

	//멀티텍스쳐 0, 1번 텍스쳐 연산을 중지
	D3DDEVICE->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	D3DDEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	D3DDEVICE->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	D3DDEVICE->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	//알파블렌딩 사용 중지
	D3DDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

bool Plane::CreateAlpha()
{
	LPBYTE pData;
	LPDWORD pDWord;
	D3DLOCKED_RECT Alphamap_locked;;

	for (int i = 0; i < MAX_TEXTURE; ++i)
		/*for(int i = 0; i < 2; ++i)*/
	{
		if (m_pAlpha[i])
		{
			m_pAlpha[i]->Release();
		}

		if (FAILED(D3DDEVICE->CreateTexture(1024, 1024, 1, 0, D3DFMT_A8R8G8B8,
											D3DPOOL_MANAGED, &m_pAlpha[i], NULL)))
		{
			return false;
		}

		memset(&Alphamap_locked, 0, sizeof(D3DLOCKED_RECT));

		if (FAILED(m_pAlpha[i]->LockRect(0, &Alphamap_locked, NULL, 0)))
		{
			return false;
		}

		pData = (LPBYTE)Alphamap_locked.pBits;

		for (int y = 0; y < 1024; ++y)
		{
			pDWord = (LPDWORD)(pData + y * Alphamap_locked.Pitch);

			for (int x = 0; x < 1024; ++x)
			{
				*(pDWord + x) = 0x00000000;
			}
		}

		if (FAILED(m_pAlpha[i]->UnlockRect(0)))
		{
			return false;
		}
	}

	return true;
}

bool Plane::DrawAlpha(Picking * pRay, int Idx)
{
	int Row, Col;

	float PixSize = (float)m_iWidth / 1024.0f;

	//브러쉬 설정
	float HalfBrushSize = (float)iBigCircle;
	float HalfSmoothSize = 10.0f;

	float EditSize = HalfBrushSize / PixSize;

	float PosU = (float)(pRay->PickPos.x + m_iWidth / 2.0f) / (float)m_iWidth;
	float PosV = 1.0f - (float)(pRay->PickPos.z + m_iDepth / 2.0f) / (float)m_iDepth;

	m_iTexPosX = (int)(PosU * 1024.0f + m_iWidth * 0.0004f);
	m_iTexPosY = (int)(PosV * 1024.0f + m_iDepth * 0.0004f);

	//브러쉬 범위 설정
	int StartPosX = (int)(((m_iTexPosX - EditSize) < 0) ? 0 : m_iTexPosX - EditSize);
	int StartPosY = (int)(((m_iTexPosY - EditSize) < 0) ? 0 : m_iTexPosY - EditSize);
	int EndPosX = (int)(((m_iTexPosX + EditSize) >= 1024) ? 1024 - 1 :
						m_iTexPosX + EditSize);
	int EndPosY = (int)(((m_iTexPosY + EditSize) >= 1024) ? 1024 - 1 :
						m_iTexPosY + EditSize);

	float Length, Smooth;

	DWORD a, ChangeColor;
	D3DLOCKED_RECT Alphamap_locked;

	memset(&Alphamap_locked, 0, sizeof(D3DLOCKED_RECT));

	//비트 열기
	if (FAILED(m_pAlpha[1]->LockRect(0, &Alphamap_locked, NULL, 0)))
	{
		return false;
	}

	//알파 비트 조작
	LPBYTE DataDST = (LPBYTE)Alphamap_locked.pBits;

	for (int z = StartPosY; z < EndPosY; ++z)
	{
		LPDWORD DWordDST = (LPDWORD)(DataDST + z * Alphamap_locked.Pitch);

		for (int x = StartPosX; x < EndPosX; ++x)
		{
			D3DXVECTOR3 Pix = D3DXVECTOR3(x * PixSize, 0.0f, z * PixSize) -
				D3DXVECTOR3(m_iTexPosX * PixSize, 0.0f, m_iTexPosY * PixSize);

			Length = (float)D3DXVec3Length(&Pix);

			if (Length <= HalfSmoothSize)
			{
				ChangeColor = 0xFFFFFFFF;
			}
			else if (Length <= HalfBrushSize)
			{
				Length -= HalfSmoothSize;

				Smooth = HalfBrushSize - HalfSmoothSize;

				ChangeColor = (DWORD)((Smooth - Length) / Smooth * 0xFF000000);

				a = (ChangeColor & 0xFF000000) >> 24;

				ChangeColor = (a << 24) | (a << 16) | (a << 8) | a;
			}
			else
			{
				continue;
			}

			//색상범위 오버플로우 확인 후 설정
			DWordDST[x] = (DWordDST[x] < ChangeColor) ? ChangeColor : DWordDST[x];
		}
	}

	//비트 닫기
	if (FAILED(m_pAlpha[1]->UnlockRect(0)))
	{
		return false;
	}

	return true;
}

void Plane::SetPickingValue(int x)
{
	m_iPickingvalue = x;
}

void Plane::SetPickingUpDown(int x)
{
	m_iUpdown = x;
}

void Plane::SetPickingBig(int x)
{
	iBigCircle = x;
}

void Plane::SetPickingSmall(int x)
{
	iSmallCircle = x;
}

int Plane::ReturnPickingValue()
{
	return m_iPickingvalue;
}

int Plane::ReturnPickingBig()
{
	return iBigCircle;
}

int Plane::ReturnPickingSmall()
{
	return iSmallCircle;
}

int Plane::ReturnBigValue()
{
	return m_iBigValue;
}

int Plane::ReturnSmallValue()
{
	return m_iSmallValue;
}

void Plane::SetBigValue(int x)
{
	m_iBigValue = x;
}

void Plane::SetSmallValue(int x)
{
	m_iSmallValue = x;
}
