#include "stdafx.h"
#include "HeightMap.h"


HeightMap::HeightMap()
	: texture(nullptr), mesh(nullptr)
{
}


HeightMap::~HeightMap()
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(mesh);
}

HRESULT HeightMap::Load(char * szFullPath, D3DXMATRIX * pmat)
{
	FILE* file = nullptr;
	std::vector<VertexPNT>	PNTList;
	PNTList.resize(VERTEX_NUM * VERTEX_NUM);
	verts.resize(VERTEX_NUM * VERTEX_NUM);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = material.Diffuse = material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (FAILED(fopen_s(&file, szFullPath, "rb"))) return E_FAIL;

	for (int z = 0; z < VERTEX_NUM; ++z)
	{
		for (int x = 0; x < VERTEX_NUM; ++x)
		{
			int index = z * VERTEX_NUM + x;
			float y = ((unsigned char)fgetc(file)) / 10.0f;

			PNTList[index].p = D3DXVECTOR3(x, y, z);
			PNTList[index].n = D3DXVECTOR3(0, 1, 0);
			PNTList[index].t = D3DXVECTOR2(x / (float)POLYGON_NUM, z / (float)POLYGON_NUM);
			
			if (pmat)
			{
				D3DXVec3TransformCoord(&PNTList[index].p, &PNTList[index].p, pmat);
			}
			verts[index] = PNTList[index].p;
		}
	}

	fclose(file);

	CreateMesh(PNTList);

	return S_OK;
}

void HeightMap::CreateMesh(std::vector<VertexPNT>& PNTList)
{
	//노말 구하기
	for (int z = 0; z < POLYGON_NUM; z++)
	{
		for (int x = 0; x < POLYGON_NUM; x++)
		{
			if (x <= 0 || x >= VERTEX_NUM) continue;
			if (z <= 0 || z >= VERTEX_NUM) continue;
			int index = z * VERTEX_NUM + x;

			int left = index - 1;
			int right = index + 1;
			int top = index - VERTEX_NUM;
			int bottom = index + VERTEX_NUM;

			D3DXVECTOR3 bt = verts[top] - verts[bottom];
			D3DXVECTOR3 lr = verts[right] - verts[left];

			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &lr, &bt);
			D3DXVec3Normalize(&n, &n);

			PNTList[index].n = n;
		}
	}

	//인덱스 리스트
	std::vector<DWORD> indexList;
	//한 그리드에 3(버텍스 3개) * 2(폴리곤 2개)
	indexList.reserve(POLYGON_NUM * POLYGON_NUM * 3 * 2);

	for (int z = 0; z < POLYGON_NUM; z++)
	{
		for (int x = 0; x < POLYGON_NUM; x++)
		{
			int _v0 = (z + 0) * VERTEX_NUM + x + 0;
			int _v1 = (z + 1) * VERTEX_NUM + x + 0;
			int _v2 = (z + 0) * VERTEX_NUM + x + 1;
			int _v3 = (z + 1) * VERTEX_NUM + x + 1;

			indexList.push_back(_v0);
			indexList.push_back(_v1);
			indexList.push_back(_v2);

			indexList.push_back(_v3);
			indexList.push_back(_v2);
			indexList.push_back(_v1);
		}
	}

	D3DXCreateMeshFVF(indexList.size(), PNTList.size(), D3DXMESH_MANAGED | D3DXMESH_32BIT, VertexPNT::FVF, D3DDEVICE, &mesh);

	VertexPNT* vertices = nullptr;
	mesh->LockVertexBuffer(0, (LPVOID*)&vertices);
	memcpy(vertices, &PNTList[0], PNTList.size() * sizeof(VertexPNT));
	mesh->UnlockVertexBuffer();

	DWORD* index = nullptr;
	mesh->LockIndexBuffer(0, (LPVOID*)&index);
	memcpy(index, &indexList[0], indexList.size() * sizeof(DWORD));
	mesh->UnlockIndexBuffer();

	DWORD* att = nullptr;
	mesh->LockAttributeBuffer(0, &att);
	ZeroMemory(att, indexList.size() / 3 * sizeof(DWORD)); //ZeroMemory를 하면 0으로 세팅됨
	mesh->UnlockAttributeBuffer();

	std::vector<DWORD> adjBuffer(mesh->GetNumFaces() * 3);
	mesh->GenerateAdjacency(0.0f, &adjBuffer[0]);

	mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, &adjBuffer[0], 0, 0, 0);
}

bool HeightMap::SetHeight(IN const float & x, OUT float & y, IN const float & z, RayCtrl& r)
{
	

	return true;
}

bool HeightMap::GetHeight(IN const float & x, OUT float & y, IN const float & z)
{
	//맵 밖으로 나갔을때
	if (x < 0 || x >= POLYGON_NUM || z < 0 || z >= POLYGON_NUM) return false;
	float col = floorf(x);
	float row = floorf(z);

	D3DXVECTOR3 _0 = GetHeightMapEntry(row, col);
	D3DXVECTOR3 _1 = GetHeightMapEntry(row, col + 1);
	D3DXVECTOR3 _2 = GetHeightMapEntry(row + 1, col);
	D3DXVECTOR3 _3 = GetHeightMapEntry(row + 1, col + 1);

	float dx = x - col;
	float dz = z - row;
	
	if (dz + dx < 1.0f) //아래 폴리곤
	{
		D3DXVECTOR3 uv = _1 - _0;
		D3DXVECTOR3 vy = _2 - _0;
		D3DXVECTOR3 dzU = uv * dz;
		D3DXVECTOR3 dxV = vy * dx;
		
		y = _0.y + (dzU + dxV).y;
		return true;
	}
	
	else //위쪽
	{
		D3DXVECTOR3 uv = _2 - _3;
		D3DXVECTOR3 vy = _1 - _3;
		D3DXVECTOR3 dxU = uv * (1.0f - dx);
		D3DXVECTOR3 dzV = vy * (1.0f - dz);

		y = _3.y + (dxU + dzV).y;
		return true;
	}
}

D3DXVECTOR3 HeightMap::GetHeightMapEntry(int row, int col)
{
	return verts[row * VERTEX_NUM + col];
}

void HeightMap::Render()
{
	D3DDEVICE->SetTexture(0, texture);
	D3DDEVICE->SetMaterial(&material);
	mesh->DrawSubset(0);
}

void HeightMap::Release()
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(mesh);
}
