#include "stdafx.h"
#include "QuadTree.h"


QuadTree::QuadTree(int x, int y)
{
	int i;

	iCenter = 0;
	for (int i = 0; i < 4; ++i)
	{
		child[i] = nullptr;
		neighbor[i] = nullptr;
	}

	parent = nullptr;

	iCorner[CORNER_TL] = 0;
	iCorner[CORNER_TR] = x - 1;
	iCorner[CORNER_BL] = x * (y - 1);
	iCorner[CORNER_BR] = x * y - 1;

	iCenter = (iCorner[CORNER_TL] + iCorner[CORNER_TR] +
		iCorner[CORNER_BL] + iCorner[CORNER_BR]) / 4;

	fRadius = 0.0f;
	isFrustum = false;
}

QuadTree::QuadTree(QuadTree* pParent)
{
	int i;
	iCenter = 0;
	parent = pParent;

	for (int i = 0; i < 4; ++i)
	{
		child[i] = nullptr;
		neighbor[i] = nullptr;
		iCorner[i] = 0;
	}

	fRadius = 0.0f;
	isFrustum = false;
}

QuadTree::~QuadTree()
{
	Destroy();
}

bool QuadTree::Build(VertexPNT * pHMap)
{
	//기존 쿼드트리
	BuildQuadTree(pHMap);
	//이웃노드
	BuildNeighborNode(this, pHMap, iCorner[CORNER_TR] + 1);

	return true;
}

int QuadTree::GetIndex(LPVOID pIB, VertexPNT * pHMap, FrustumCulling * pFrustum, float fLODRatio)
{
	FrustumCull(pHMap, pFrustum);

	return GenTriIndex(0, pIB, pHMap, pFrustum, fLODRatio);
}

float QuadTree::GetDistance(D3DXVECTOR3 * v1, D3DXVECTOR3 * v2)
{
	return D3DXVec3Length(&(*v2 - *v1));
}

int QuadTree::GetLODLevel(VertexPNT * pHMap, D3DXVECTOR3 * pCam, float fLODRatio)
{
	float d = GetDistance((D3DXVECTOR3*)(pHMap + iCenter), pCam);

	return max((int)(d*fLODRatio), 1);
}

QuadTree* QuadTree::AddChild(int iTL, int iTR, int iBL, int iBR)
{
	QuadTree* pChild;

	pChild = new QuadTree(this);
	pChild->SetCorners(iTL, iTR, iBL, iBR);

	return pChild;
}

bool QuadTree::SetCorners(int iTL, int iTR, int iBL, int iBR)
{
iCorner[CORNER_TL] = iTL;
iCorner[CORNER_TR] = iTR;
iCorner[CORNER_BL] = iBL;
iCorner[CORNER_BR] = iBR;

iCenter = (iCorner[CORNER_TL] + iCorner[CORNER_TR] + iCorner[CORNER_BL] + iCorner[CORNER_BR]) / 4;

return true;
}

bool QuadTree::SubDivide()
{
	int iTopEdgeCenter, iBottomEdgeCenter, iLeftEdgeCenter, iRightEdgeCenter, iCenterPoint;

	iTopEdgeCenter = (iCorner[CORNER_TL] + iCorner[CORNER_TR]) / 2;
	iBottomEdgeCenter = (iCorner[CORNER_BL] + iCorner[CORNER_BR]) / 2;
	iLeftEdgeCenter = (iCorner[CORNER_TL] + iCorner[CORNER_BL]) / 2;
	iRightEdgeCenter = (iCorner[CORNER_TR] + iCorner[CORNER_BR]) / 2;
	iCenterPoint = (iCorner[CORNER_TL] + iCorner[CORNER_TR] + iCorner[CORNER_BL] + iCorner[CORNER_BR]) / 4;

	if ((iCorner[CORNER_TR] - iCorner[CORNER_TL]) <= 1) return FALSE;

	child[CORNER_TL] = AddChild(iCorner[CORNER_TL], iTopEdgeCenter, iLeftEdgeCenter, iCenterPoint);
	child[CORNER_TR] = AddChild(iTopEdgeCenter, iCorner[CORNER_TR], iCenterPoint, iRightEdgeCenter);
	child[CORNER_BL] = AddChild(iLeftEdgeCenter, iCenterPoint, iCorner[CORNER_BL], iBottomEdgeCenter);
	child[CORNER_BR] = AddChild(iCenterPoint, iRightEdgeCenter, iBottomEdgeCenter, iCorner[CORNER_BR]);

	return true;
}

bool QuadTree::IsVisible(VertexPNT * pHMap, D3DXVECTOR3 * pCam, float fLODRatio)
{
	return ((iCorner[CORNER_TR] - iCorner[CORNER_TL]) <= GetLODLevel(pHMap, pCam, fLODRatio));
}

int QuadTree::GenTriIndex(int iTriangles, LPVOID pIndex, VertexPNT * pHMap, FrustumCulling * pFrustum, float fLODRatio)
{
	if (isFrustum)
	{
		isFrustum = false;
		return iTriangles;
	}

	LPWORD p = ((LPWORD)pIndex) + iTriangles * 3;

	if (IsVisible(pHMap, pFrustum->GetPos(), fLODRatio))
	{
		//최하위 노드라면 부분분할 불가능
		if (iCorner[CORNER_TR] - iCorner[CORNER_TL] <= 1)
		{
			//좌측 상단 삼각형
			*p++ = iCorner[0];
			*p++ = iCorner[1];
			*p++ = iCorner[2];
			iTriangles++;
			//우측 하단 삼각형
			*p++ = iCorner[2];
			*p++ = iCorner[1];
			*p++ = iCorner[3];
			iTriangles++;

			return iTriangles;
		}
		//이웃 노드가 출력 가능한지 검사
		BOOL b[4] = { FALSE, FALSE, FALSE, FALSE };
		if (neighbor[EDGE_UP])
		{
			b[EDGE_UP] = neighbor[EDGE_UP]->IsVisible(pHMap, pFrustum->GetPos(), fLODRatio);
		}
		if (neighbor[EDGE_DOWN])
		{
			b[EDGE_DOWN] = neighbor[EDGE_DOWN]->IsVisible(pHMap, pFrustum->GetPos(), fLODRatio);
		}
		if (neighbor[EDGE_LEFT])
		{
			b[EDGE_LEFT] = neighbor[EDGE_LEFT]->IsVisible(pHMap, pFrustum->GetPos(), fLODRatio);
		}
		if (neighbor[EDGE_RIGHT])
		{
			b[EDGE_RIGHT] = neighbor[EDGE_RIGHT]->IsVisible(pHMap, pFrustum->GetPos(), fLODRatio);
		}

		//이웃노드가 모두 출력 가능하다는 것은 현재 노드와 이웃 노드가 같은 LOD(부분 분할 필요 X)
		if (b[EDGE_UP] && b[EDGE_DOWN] && b[EDGE_LEFT] && b[EDGE_RIGHT])
		{
			*p++ = iCorner[0];
			*p++ = iCorner[1];
			*p++ = iCorner[2];
			iTriangles++;

			*p++ = iCorner[2];
			*p++ = iCorner[1];
			*p++ = iCorner[3];
			iTriangles++;

			return iTriangles;
		}

		int n;
		if (!b[EDGE_UP]) //상단노드가 출력불가 (부분분할 필요)
		{
			n = (iCorner[CORNER_TL] + iCorner[CORNER_TR]) / 2;
			*p++ = iCenter;
			*p++ = iCorner[CORNER_TL];
			*p++ = n;
			iTriangles++;

			*p++ = iCenter;
			*p++ = n;
			*p++ = iCorner[CORNER_TR];
			iTriangles++;
		}
		else
		{
			*p++ = iCenter;
			*p++ = iCorner[CORNER_TL];
			*p++ = iCorner[CORNER_TR];
			iTriangles++;
		}

		if (!b[EDGE_DOWN])
		{
			n = (iCorner[CORNER_BL] + iCorner[CORNER_BR]) / 2;

			*p++ = iCenter;
			*p++ = iCorner[CORNER_BR];
			*p++ = n;
			iTriangles++;

			*p++ = iCenter;
			*p++ = n;
			*p++ = iCorner[CORNER_BL];
			iTriangles++;
		}
		else
		{
			*p++ = iCenter;
			*p++ = iCorner[CORNER_BR];
			*p++ = iCorner[CORNER_BL];
			iTriangles++;
		}

		if (!b[EDGE_LEFT])
		{
			n = (iCorner[CORNER_TL] + iCorner[CORNER_BL]) / 2;

			*p++ = iCenter;
			*p++ = iCorner[CORNER_BL];
			*p++ = n;
			iTriangles++;

			*p++ = iCenter;
			*p++ = n;
			*p++ = iCorner[CORNER_TL];
			iTriangles++;
		}
		else
		{
			*p++ = iCenter;
			*p++ = iCorner[CORNER_BL];
			*p++ = iCorner[CORNER_TL];
			iTriangles++;
		}

		if (!b[EDGE_RIGHT])
		{
			n = (iCorner[CORNER_TR] + iCorner[CORNER_BR]) / 2;

			*p++ = iCenter;
			*p++ = iCorner[CORNER_TR];
			*p++ = n;
			iTriangles++;

			*p++ = iCenter;
			*p++ = n;
			*p++ = iCorner[CORNER_BR];
			iTriangles++;
		}
		else
		{
			*p++ = iCenter;
			*p++ = iCorner[CORNER_TR];
			*p++ = iCorner[CORNER_BR];
			iTriangles++;
		}

		return iTriangles;
	}

	//자식 노드 검색
	if (child[CORNER_TL])
	{
		iTriangles = child[CORNER_TL]->GenTriIndex(iTriangles, pIndex, pHMap, pFrustum, fLODRatio);
	}
	if (child[CORNER_TR])
	{
		iTriangles = child[CORNER_TR]->GenTriIndex(iTriangles, pIndex, pHMap, pFrustum, fLODRatio);
	}
	if (child[CORNER_BL])
	{
		iTriangles = child[CORNER_BL]->GenTriIndex(iTriangles, pIndex, pHMap, pFrustum, fLODRatio);
	}
	if (child[CORNER_BR])
	{
		iTriangles = child[CORNER_BR]->GenTriIndex(iTriangles, pIndex, pHMap, pFrustum, fLODRatio);
	}

	return iTriangles;
}

void QuadTree::Destroy()
{
	for (int i = 0; i < 4; ++i)
	{
		if (child[i] != nullptr)
		{
			delete child[i];

			child[i] = nullptr;
		}
	}
}

int QuadTree::IsInFrustum(VertexPNT * pHMap, FrustumCulling * pFrustum)
{
	BOOL b[4];
	BOOL bInSphere;

	//프러스텀 안에 있는지 검사
	bInSphere = pFrustum->IsInSphere((D3DXVECTOR3*)(pHMap + iCenter), fRadius);

	if (!bInSphere) return FRUSTUM_OUT;

	//쿼드트리의 4개 경계 절두체 테스트
	b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHMap + iCorner[0]));
	b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHMap + iCorner[1]));
	b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHMap + iCorner[2]));
	b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHMap + iCorner[3]));

	//4개 모두 절두체 안에 있는 경우
	if ((b[0] + b[1] + b[2] + b[3]) == 4)
	{
		return FRUSTUM_COMPLETELY_IN;
	}

	//일부만 절두체에 있는 경우
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::FrustumCull(VertexPNT * pHMap, FrustumCulling * pFrustum)
{
	int ret;

	ret = IsInFrustum(pHMap, pFrustum);

	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:
		isFrustum = false;
		break;
	case FRUSTUM_PARTIALLY_IN:
		isFrustum = false;
		break;
	case FRUSTUM_OUT:
		isFrustum = true;
		break;
	}

	if (child[0]) child[0]->FrustumCull(pHMap, pFrustum);
	if (child[1]) child[1]->FrustumCull(pHMap, pFrustum);
	if (child[2]) child[2]->FrustumCull(pHMap, pFrustum);
	if (child[3]) child[3]->FrustumCull(pHMap, pFrustum);
}

void QuadTree::BuildNeighborNode(QuadTree* pRoot, VertexPNT * pHMap, int ix)
{
	int n;
	int a, b, c, d;
	
	for (int i = 0; i < 4; ++i)
	{
		a = iCorner[0];
		b = iCorner[1];
		c = iCorner[2];
		d = iCorner[3];
		//이웃노드 4개의 코너값 구함
		n = GetNodeIndex(i, ix, a, b, c, d);

		if (n >= 0)
		{
			neighbor[i] = pRoot->FindNode(pHMap, a, b, c, d);
		}
	}

	if (child[0])
	{
		child[0]->BuildNeighborNode(pRoot, pHMap, ix);
		child[1]->BuildNeighborNode(pRoot, pHMap, ix);
		child[2]->BuildNeighborNode(pRoot, pHMap, ix);
		child[3]->BuildNeighborNode(pRoot, pHMap, ix);
	}
}

bool QuadTree::BuildQuadTree(VertexPNT * pHMap)
{
	if (SubDivide())
	{
		//좌측 상단과 우측 하단의 거리를 구한다
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHMap + iCorner[CORNER_TL])) - *((D3DXVECTOR3*)(pHMap + iCorner[CORNER_BR]));

		//v의 거리값이 이 노드를 감싸는 경계구의 지름이므로 2로 나눠 반지름을 구한다
		fRadius = D3DXVec3Length(&v) / 2.0f;

		child[CORNER_TL]->BuildQuadTree(pHMap);
		child[CORNER_TR]->BuildQuadTree(pHMap);
		child[CORNER_BL]->BuildQuadTree(pHMap);
		child[CORNER_BR]->BuildQuadTree(pHMap);
	}

	return true;
}

int QuadTree::GetNodeIndex(int ed, int ix, int & a, int & b, int & c, int & d)
{
	int n, A, B, C, D, gap;

	A = a;
	B = b;
	C = c;
	D = d;
	gap = B - A;

	switch (ed)
	{
		case EDGE_UP:
			a = A - ix * gap;
			b = B - ix * gap;
			c = A;
			d = B;
			break;
		case EDGE_DOWN:
			a = C;
			b = D;
			c = C + ix * gap;
			d = D + ix * gap;
			break;
		case EDGE_LEFT:
			a = A - gap;
			b = A;
			c = C - gap;
			d = C;
			break;
		case EDGE_RIGHT:
			a = B;
			b = B + gap;
			c = D;
			d = D + gap;
			break;
	}

	n = (a + b + c + d) / 4;

	if (!(IsInRange(n, 0, ix * ix - 1))) return -1;
	return n;
}

QuadTree* QuadTree::FindNode(VertexPNT * pHMap, int a, int b, int c, int d)
{
	QuadTree* p = nullptr;

	//코너값과 일치한다면 노드 리턴
	if ((iCorner[0] == a) && (iCorner[1] == b) && (iCorner[2] == c) && (iCorner[3] == d))
		return this;

	if (child[0])
	{
		RECT rc;
		POINT pt;

		int n = (a + b + c + d) / 4;

		//현재 맵상에서의 위치
		pt.x = (int)pHMap[n].p.x;
		pt.y = (int)pHMap[n].p.z;

		//4개의 코너를 기준으로 자식노드의 맵 점유범위를 얻음
		SetRect(&rc, (int)pHMap[child[0]->iCorner[CORNER_TL]].p.x,
				(int)pHMap[child[0]->iCorner[CORNER_TL]].p.z,
				(int)pHMap[child[0]->iCorner[CORNER_BR]].p.x,
				(int)pHMap[child[0]->iCorner[CORNER_BR]].p.z);
		//pt가 점유범위 안에 있다면 자식노드로 들어간다
		if (IsInRect(&rc, pt)) return child[0]->FindNode(pHMap, a, b, c, d);

		//4개의 코너를 기준으로 자식노드의 맵 점유범위를 얻음
		SetRect(&rc, (int)pHMap[child[1]->iCorner[CORNER_TL]].p.x,
				(int)pHMap[child[1]->iCorner[CORNER_TL]].p.z,
				(int)pHMap[child[1]->iCorner[CORNER_BR]].p.x,
				(int)pHMap[child[1]->iCorner[CORNER_BR]].p.z);

		if (IsInRect(&rc, pt)) return child[2]->FindNode(pHMap, a, b, c, d);

		//4개의 코너를 기준으로 자식노드의 맵 점유범위를 얻음
		SetRect(&rc, (int)pHMap[child[2]->iCorner[CORNER_TL]].p.x,
				(int)pHMap[child[2]->iCorner[CORNER_TL]].p.z,
				(int)pHMap[child[2]->iCorner[CORNER_BR]].p.x,
				(int)pHMap[child[2]->iCorner[CORNER_BR]].p.z);

		if (IsInRect(&rc, pt)) return child[2]->FindNode(pHMap, a, b, c, d);

		//4개의 코너를 기준으로 자식노드의 맵 점유범위를 얻음
		SetRect(&rc, (int)pHMap[child[3]->iCorner[CORNER_TL]].p.x,
				(int)pHMap[child[3]->iCorner[CORNER_TL]].p.z,
				(int)pHMap[child[3]->iCorner[CORNER_BR]].p.x,
				(int)pHMap[child[3]->iCorner[CORNER_BR]].p.z);

		if (IsInRect(&rc, pt)) return child[2]->FindNode(pHMap, a, b, c, d);
	}

	return nullptr;
}

bool QuadTree::IsInRect(RECT * rect, POINT pt)
{
	if ((rect->left <= pt.x) && (pt.x <= rect->right) && (rect->bottom <= pt.y) && (pt.y <= rect->top))
		return true;
	return false;
}
