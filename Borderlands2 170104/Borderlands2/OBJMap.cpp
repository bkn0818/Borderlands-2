#include "stdafx.h"
#include "OBJMap.h"


OBJMap::OBJMap()
{
}


OBJMap::~OBJMap()
{
}

void OBJMap::Load(char * szFullPath, D3DXMATRIX * pmat)
{

	std::vector<D3DXVECTOR3> vecV;

	FILE* fp = NULL;
	fopen_s(&fp, szFullPath, "r");

	while (true)
	{
		if (feof(fp))
			break;

		char szTemp[1024];

		fgets(szTemp, 1024, fp);

		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{
		}
		else if (szTemp[0] == 'g')
		{
		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == 't')
			{
			}
			else if (szTemp[1] == 'n')
			{
			}
			else // v
			{
				float x, y, z;
				sscanf(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szTemp[0] == 'u')
		{
		}
		else if (szTemp[0] == 'f')
		{
			int aIndex[3];
			sscanf(szTemp, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
				   &aIndex[0], &aIndex[1], &aIndex[2]);

			for (int i = 0; i < 3; ++i)
			{
				D3DXVECTOR3 p;
				p = vecV[aIndex[i] - 1];

				if (pmat)
				{
					D3DXVec3TransformCoord(&p, &p, pmat);
				}
				m_vecVertex.push_back(p);
			}
		}
	}

	fclose(fp);


}

bool OBJMap::GetHeight(IN const float & x, OUT float & y, IN const float & z)
{
	D3DXVECTOR3 vRayOrig(x, 1000, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);
	float u, v, d;
	for (size_t i = 0; i < m_vecVertex.size(); i += 3)
	{
		if (D3DXIntersectTri(&m_vecVertex[i],
							 &m_vecVertex[i + 1],
							 &m_vecVertex[i + 2],
							 &vRayOrig,
							 &vRayDir, &u, &v, &d))
		{
			y = 1000 - d;
			return true;
		}
	}
	return false;
}
