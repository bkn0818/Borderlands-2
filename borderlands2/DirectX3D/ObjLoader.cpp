#include "stdafx.h"
#include "OBJLoader.h"


OBJLoader::OBJLoader()
{
}

OBJLoader::~OBJLoader()
{
	mapMaterialTexture.clear();
}

void OBJLoader::SetPath(char * path)
{
}

void OBJLoader::Load(IN char* szFullPath, IN D3DXMATRIX* pmat, OUT std::vector<Group*>& vecGroup)
{
	FILE* file = nullptr;

	std::vector<D3DXVECTOR3>	vertexList;
	std::vector<D3DXVECTOR3>	normalList;
	std::vector<D3DXVECTOR2>	textureList;
	std::vector<VertexPNT>		totalList;
	std::string					materialName;

	fopen_s(&file, szFullPath, "r");

	while (!feof(file))
	{
		char str[1024];
		fgets(str, 1024, file);

		if (str[0] == '#') continue;
		else if (str[0] == 'm')
		{
			char fullPath[1024];
			sscanf(str, "%*s %s", &fullPath);
			LoadMtlLib(fullPath);
		}
		else if (str[0] == 'v')
		{
			if (str[1] == 't')
			{
				D3DXVECTOR2 t;
				sscanf(str, "%*s %f %f %*f", &t.x, &t.y);
				textureList.push_back(t);
			}
			else if (str[1] == 'n')
			{
				D3DXVECTOR3 normal;
				sscanf(str, "%*s %f %f %f", &normal.x, &normal.y, &normal.z);
				normalList.push_back(normal);
			}
			else
			{
				D3DXVECTOR3 v;
				sscanf(str, "%*s %f %f %f", &v.x, &v.y, &v.z);
				vertexList.push_back(v);
			}
		}
		else if (str[0] == 'g')
		{
			if (totalList.empty()) continue;
			Group* group = new Group();
			group->SetPNTVerts(totalList);
			group->SetVertexBuffer();
			group->SetMaterialTexture(mapMaterialTexture[materialName]);
			vecGroup.push_back(group);
			totalList.clear();
		}
		else if (str[0] == 'u')
		{
			char scanedString[1024];
			sscanf(str, "%*s %s", scanedString);
			materialName = scanedString;
		}
		else if (str[0] == 'f')
		{
			int index[3][3];
			sscanf_s(str, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
					 &index[0][0], &index[0][1], &index[0][2],
					 &index[1][0], &index[1][1], &index[1][2],
					 &index[2][0], &index[2][1], &index[2][2]);
			for (int i = 0; i < 3; i++)
			{
				VertexPNT v;

				v.p = vertexList[index[i][0] - 1];
				v.t = textureList[index[i][1] - 1];
				v.n = normalList[index[i][2] - 1];
				if (pmat)
				{
					D3DXVec3TransformCoord(&v.p, &v.p, pmat);
					D3DXVec3TransformCoord(&v.n, &v.n, pmat);
				}
				totalList.push_back(v);
			}
		}
	}	

	for each(auto it in mapMaterialTexture)
	{
		SAFE_RELEASE(it.second);
	}
	mapMaterialTexture.clear();
	fclose(file);
}

void OBJLoader::LoadMtlLib(char * szFullPath)
{
	FILE* file = NULL;

	int i = 0;

	fopen_s(&file, szFullPath, "r");
	if (fopen_s(&file, szFullPath, "r") == E_FAIL)
	{
		return;
	}

	std::string sMtlName;

	while (true)
	{
		if (feof(file)) break;
		char szTemp[1024];
		fgets(szTemp, 1024, file);
		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'n')
		{
			char szMtlName[1024];
			sscanf(szTemp, "%*s %s", szMtlName);
			sMtlName = szMtlName;
			SAFE_RELEASE(mapMaterialTexture[sMtlName]);
			mapMaterialTexture[sMtlName] = new MaterialTexture;
			mapMaterialTexture[sMtlName]->SetMTRef(i++);
		}
		else if (szTemp[0] == 'K')
		{
			if (szTemp[1] == 'a')
			{
				float r, g, b;
				sscanf(szTemp, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = mapMaterialTexture[sMtlName]->GetMaterial();
				stMtl.Ambient.r = r;
				stMtl.Ambient.g = g;
				stMtl.Ambient.b = b;
				stMtl.Ambient.a = 1.0f;
			}
			else if (szTemp[1] == 'd')
			{
				float r, g, b;
				sscanf(szTemp, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = mapMaterialTexture[sMtlName]->GetMaterial();
				stMtl.Diffuse.r = r;
				stMtl.Diffuse.g = g;
				stMtl.Diffuse.b = b;
				stMtl.Diffuse.a = 1.0f;
			}
			else if (szTemp[1] == 's')
			{
				float r, g, b;
				sscanf(szTemp, "%*s %f %f %f", &r, &g, &b);
				D3DMATERIAL9& stMtl = mapMaterialTexture[sMtlName]->GetMaterial();
				stMtl.Specular.r = r;
				stMtl.Specular.g = g;
				stMtl.Specular.b = b;
				stMtl.Specular.a = 1.0f;
			}
		}
		else if (szTemp[0] == 'm')
		{
			char szTexturePath[1024];
			sscanf(szTemp, "%*s %s", szTexturePath);
			mapMaterialTexture[sMtlName]->SetTexture(g_pTextureManager->GetTexture(szTexturePath));
		}
	}
	fclose(file);
}

LPD3DXMESH OBJLoader::LoadMesh(IN char * filename, IN D3DXMATRIX * pMat, OUT std::vector<MaterialTexture*> &mtList)
{
	std::vector<D3DXVECTOR3>	vList;
	std::vector<D3DXVECTOR2>	tList;
	std::vector<D3DXVECTOR3>	nList;
	std::vector<VertexPNT>		pntList;
	std::vector<DWORD>			attList;
	std::string					materialName;

	FILE* file;
	fopen_s(&file, filename, "r");

	while (!feof(file))
	{
		char str[1024];
		fgets(str, 1024, file);

		if (str[0] == '#') continue;
		else if (str[0] == 'm')
		{
			char fullPath[1024];
			sscanf(str, "%*s %s", &fullPath);
			LoadMtlLib(fullPath);
			mtList.resize(mapMaterialTexture.size());
			for each(auto p in mapMaterialTexture)
			{
				//mapMaterialTexture에서 레퍼런스 순서대로 mt리스트 각 번지마다 메테리얼텍스처 넣음
				mtList[p.second->GetMTRef()] = p.second;
			}
		}
		else if (str[0] == 'v')
		{
			if (str[1] == 't')
			{
				D3DXVECTOR2 t;
				sscanf(str, "%*s %f %f %*f", &t.x, &t.y);
				tList.push_back(t);
			}
			else if (str[1] == 'n')
			{
				D3DXVECTOR3 n;
				sscanf(str, "%*s %f %f %f", &n.x, &n.y, &n.z);
				nList.push_back(n);
			}
			else
			{
				D3DXVECTOR3 v;
				sscanf(str, "%*s %f %f %f", &v.x, &v.y, &v.z);
				vList.push_back(v);
			}
		}
		else if (str[0] == 'g')
		{
			if (pntList.empty()) continue;
		}
		else if (str[0] == 'u')
		{
			char scanedString[1024];
			sscanf(str, "%*s %s", scanedString);
			materialName = scanedString;
		}
		else if (str[0] == 'f')
		{
			int index[3][3];
			sscanf_s(str, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
					 &index[0][0], &index[0][1], &index[0][2],
					 &index[1][0], &index[1][1], &index[1][2],
					 &index[2][0], &index[2][1], &index[2][2]);

			//attribute list에 메테리얼의 인덱스를 넣음
			attList.push_back(mapMaterialTexture[materialName]->GetMTRef());
			for (int i = 0; i < 3; i++)
			{
				VertexPNT v;

				v.p = vList[index[i][0] - 1];
				v.t = tList[index[i][1] - 1];
				v.n = nList[index[i][2] - 1];
				if (pMat)
				{
					D3DXVec3TransformCoord(&v.p, &v.p, pMat);
					D3DXVec3TransformCoord(&v.n, &v.n, pMat);
				}
				pntList.push_back(v);
			}
		}
	}

	fclose(file);

	LPD3DXMESH mesh = nullptr;

	//폴리곤 갯수, 정점 갯수, 메쉬 옵션, FVF, 디바이스, (OUT)메쉬
	D3DXCreateMeshFVF(pntList.size() / 3, pntList.size(), D3DXMESH_MANAGED, VertexPNT::FVF, D3DDEVICE, &mesh);

	VertexPNT* v;
	mesh->LockVertexBuffer(0, (LPVOID*)&v);
	//임시 변수에 PNTList 첫번째부터 끝까지 복사
	memcpy(v, &pntList[0], pntList.size() * sizeof(VertexPNT));
	mesh->UnlockVertexBuffer();

	//DWORD로 하면 오류남
	WORD* index;
	mesh->LockIndexBuffer(0, (LPVOID*)&index);
	for (int i = 0; i < pntList.size(); ++i) index[i] = i;
	mesh->UnlockIndexBuffer();

	DWORD* att;
	mesh->LockAttributeBuffer(0, &att);
	//att에 0부터 끝까지 복사
	memcpy(att, &attList[0], attList.size() * sizeof(DWORD));
	mesh->UnlockAttributeBuffer();

	//adjacency버퍼 : 인접 폴리곤의 갯수 이므로 * 3을 해준다
	std::vector<DWORD> adjBuffer(mesh->GetNumFaces() * 3);
	//GenerateAdjacency : 입실론(인접한 폴리곤 사이의 거리), adjBuffer
	mesh->GenerateAdjacency(0.0f, &adjBuffer[0]);

	mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, &adjBuffer[0], 0, 0, 0);

	return mesh;
}
