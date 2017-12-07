#include "stdafx.h"
#include "OBJMap.h"


OBJMap::OBJMap()
{
}


OBJMap::~OBJMap()
{
}

void OBJMap::Load(char * szFullPath, D3DXMATRIX pmat)
{
	FILE* file;

	std::vector<D3DXVECTOR3> vertexList;

	fopen_s(&file, szFullPath, "r");
	if ((fopen_s(&file, szFullPath, "r")) == E_FAIL) return;

	while (!feof(file))
	{
		char line[1024];
		fgets(line, 1024, file);

		if (line[0] == '#') continue;
		else if (line[0] == 'm')
		{

		}
		else if (line[0] == 'g')
		{

		}
		else if (line[0] == 'v')
		{
			if (line[1] == 't')
			{

			}
			else if (line[1] == 'n')
			{

			}
			else
			{
				D3DXVECTOR3 v;
				sscanf(line, "%*s %f %f %f", &v.x, &v.y, &v.z);
				D3DXVec3TransformCoord(&v, &v, &pmat);
				vertexList.push_back(v);
			}
		}
		else if (line[0] == 'f')
		{
			int index[3];
			sscanf(line, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
					 &index[0], &index[1], &index[2]);
			for (int i = 0; i < 3; i++)
			{
				D3DXVECTOR3 v;
				v = vertexList[index[i] - 1];
				m_vecVertex.push_back(v);
			}
		}
	}

	fclose(file);
}

bool OBJMap::GetHeight(IN const float & x, OUT float & y, IN const float & z)
{
	float u, v, distance;
	D3DXVECTOR3 vRayOrig(x, 1000, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);

	for (int i = 0; i < m_vecVertex.size(); i += 3)
	{
		if (D3DXIntersectTri(&m_vecVertex[i + 0],
							 &m_vecVertex[i + 1],
							 &m_vecVertex[i + 2],
							 &vRayOrig,
							 &vRayDir,
							 &u,
							 &v,
							 &distance))
		{
			y = 1000 - distance;
			return true;
		}

		
	}

	return false;
}
