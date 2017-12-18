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

void HeightMap::Load(char * szFullPath, D3DXMATRIX * pmat)
{
	FILE* file = nullptr;
	std::vector<VertexPNT>	PNTList;
	PNTList.resize(VERTEX_NUM * VERTEX_NUM);
	verts.resize(VERTEX_NUM * VERTEX_NUM);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = material.Diffuse = material.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	fopen_s(&file, szFullPath, "rb");

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

bool HeightMap::SetHeight(int num, float height)
{
	verts[num].y = height;

	return true;
}

bool HeightMap::GetHeight(IN const float & x, OUT float & y, IN const float & z)
{
	if (x < 0 || z < 0 || x > POLYGON_NUM || z > POLYGON_NUM) {
		y = 0.0f;
		return false;
	}

	int intX = (int)x;
	int intZ = (int)z;
	float dx = x - intX;
	float dz = z - intZ;

	int _0 = (intZ + 0) * VERTEX_NUM + intX + 0;
	int _1 = (intZ + 1) * VERTEX_NUM + intX + 0;
	int _2 = (intZ + 0) * VERTEX_NUM + intX + 1;
	int _3 = (intZ + 1) * VERTEX_NUM + intX + 1;

	if (dx + dz < 1.0f) 
	{
		D3DXVECTOR3 v01 = verts[_1] - verts[_0];
		D3DXVECTOR3 v02 = verts[_2] - verts[_0];

		y = verts[_0].y + (v01 * dz + v02 * dx).y;
	}
	else
	{
		dx = 1 - dx;
		dz = 1 - dz;

		D3DXVECTOR3 v31 = verts[_1] - verts[_3];
		D3DXVECTOR3 v32 = verts[_2] - verts[_3];

		y = verts[_3].y + (v31 * dx + v32 * dz).y;
	}
	return true;
}

D3DXVECTOR3 HeightMap::GetHeightMapEntry(int row, int col)
{
	return verts[row * VERTEX_NUM + col];
}

D3DXVECTOR3& HeightMap::GetCurrentPolygon(int num)
{
	return verts[num];
}

void HeightMap::Render()
{
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	D3DDEVICE->SetTexture(0, texture);
	D3DDEVICE->SetMaterial(&material);
	mesh->DrawSubset(0);
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, false);
}

void HeightMap::Release()
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(mesh);
}
