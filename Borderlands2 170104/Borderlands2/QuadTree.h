#pragma once
#include "resource.h"
#include "FrustomCulling.h"

class QuadTree
{
	//쿼드트리에 보관되는 4개의 상수값
	enum CornerType
	{
		CORNER_TL, 
		CORNER_TR,
		CORNER_BL, 
		CORNER_BR
	};

	//쿼드트리(프러스텀컬링)의 위치
	enum QuadLocation
	{
		FRUSTUM_OUT = 0,
		FRUSTUM_PARTIALLY_IN,
		FRUSTUM_COMPLETELY_IN,
		FRUSTUM_UNKNOWN
	};

	//이웃 노드 처리용 상수값
	enum
	{
		EDGE_UP, 
		EDGE_DOWN, 
		EDGE_LEFT, 
		EDGE_RIGHT
	};

private:
	QuadTree*	child[4];
	QuadTree*	parent;
	QuadTree*	neighbor[4];

	int			iCenter;
	int			iCorner[4];

	bool		isFrustum;
	float		fRadius;

	//자식 노드 추가
	QuadTree* AddChild(int iTL, int iTR, int iBL, int iBR);
	//코너값 설정
	bool SetCorners(int iTL, int iTR, int iBL, int iBR);
	//쿼드트리를 4개의 하위 트리로 부분적 분할
	bool SubDivide();
	//현재 노드가 LOD등급으로 봤을 때 출력 가능한 노드인지 검사
	bool IsVisible(VertexPNT* pHMap, D3DXVECTOR3* pCam, float fLODRatio);
	//출력할 폴리곤의 인덱스 생성
	int GenTriIndex(int iTriangles, LPVOID pIndex, VertexPNT* pHMap, FrustumCulling* pFrustum, float fLODRatio);

	void Destroy();
	//현재 노드가 절두체에 포함 되는지?
	int IsInFrustum(VertexPNT* pHMap, FrustumCulling* pFrustum);
	//검사 결과에 따라 컬링
	void FrustumCull(VertexPNT* pHMap, FrustumCulling* pFrustum);
	//이웃 노드 만들기(T-Junction 방지)
	void BuildNeighborNode(QuadTree* pRoot, VertexPNT* pHMap, int ix);
	//쿼드트리 생성(이웃 노드 생성 전)
	bool BuildQuadTree(VertexPNT* pHMap);
	//4방향의 이웃 노드 인덱스를 구함
	int GetNodeIndex(int ed, int ix, int& a, int& b, int&c, int& d);
	//쿼드트리에서 4개 코너값과 일치하는 노드 검색
	QuadTree* FindNode(VertexPNT* pHMap, int a, int b, int c, int d);
	//pt가 렉트 안에 포함되는지 검사
	bool IsInRect(RECT* rect, POINT pt);
	//두 점 사이에 위치하는지 검사
	int IsInRange(int value, int r0, int r1)
	{
		if ((r0 <= value) && (value <= r1)) return 1;
		else return 0;
	}

public:
	//최초 루트노드 생성자
	QuadTree(int x, int y);
	//하위 자식노드 생성자
	QuadTree(QuadTree* pParent);
	~QuadTree();

	//쿼드트리 구축
	bool Build(VertexPNT* pHMap);
	//삼각형의 인덱스를 만들고 출력할 삼각형 개수를 리턴
	int GetIndex(LPVOID pIB, VertexPNT* pHMap, FrustumCulling* pFrustum, float fLODRatio);
	//카메라와 노드의 중앙 정점과의 거리
	float GetDistance(D3DXVECTOR3* v1, D3DXVECTOR3* v2);
	//거리값을 기준으로 LOD값 계산
	int GetLODLevel(VertexPNT* pHMap, D3DXVECTOR3* pCam, float fLODRatio);
};

