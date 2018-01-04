#pragma once
#include "resource.h"
#include "FrustomCulling.h"

class QuadTree
{
	//����Ʈ���� �����Ǵ� 4���� �����
	enum CornerType
	{
		CORNER_TL, 
		CORNER_TR,
		CORNER_BL, 
		CORNER_BR
	};

	//����Ʈ��(���������ø�)�� ��ġ
	enum QuadLocation
	{
		FRUSTUM_OUT = 0,
		FRUSTUM_PARTIALLY_IN,
		FRUSTUM_COMPLETELY_IN,
		FRUSTUM_UNKNOWN
	};

	//�̿� ��� ó���� �����
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

	//�ڽ� ��� �߰�
	QuadTree* AddChild(int iTL, int iTR, int iBL, int iBR);
	//�ڳʰ� ����
	bool SetCorners(int iTL, int iTR, int iBL, int iBR);
	//����Ʈ���� 4���� ���� Ʈ���� �κ��� ����
	bool SubDivide();
	//���� ��尡 LOD������� ���� �� ��� ������ ������� �˻�
	bool IsVisible(VertexPNT* pHMap, D3DXVECTOR3* pCam, float fLODRatio);
	//����� �������� �ε��� ����
	int GenTriIndex(int iTriangles, LPVOID pIndex, VertexPNT* pHMap, FrustumCulling* pFrustum, float fLODRatio);

	void Destroy();
	//���� ��尡 ����ü�� ���� �Ǵ���?
	int IsInFrustum(VertexPNT* pHMap, FrustumCulling* pFrustum);
	//�˻� ����� ���� �ø�
	void FrustumCull(VertexPNT* pHMap, FrustumCulling* pFrustum);
	//�̿� ��� �����(T-Junction ����)
	void BuildNeighborNode(QuadTree* pRoot, VertexPNT* pHMap, int ix);
	//����Ʈ�� ����(�̿� ��� ���� ��)
	bool BuildQuadTree(VertexPNT* pHMap);
	//4������ �̿� ��� �ε����� ����
	int GetNodeIndex(int ed, int ix, int& a, int& b, int&c, int& d);
	//����Ʈ������ 4�� �ڳʰ��� ��ġ�ϴ� ��� �˻�
	QuadTree* FindNode(VertexPNT* pHMap, int a, int b, int c, int d);
	//pt�� ��Ʈ �ȿ� ���ԵǴ��� �˻�
	bool IsInRect(RECT* rect, POINT pt);
	//�� �� ���̿� ��ġ�ϴ��� �˻�
	int IsInRange(int value, int r0, int r1)
	{
		if ((r0 <= value) && (value <= r1)) return 1;
		else return 0;
	}

public:
	//���� ��Ʈ��� ������
	QuadTree(int x, int y);
	//���� �ڽĳ�� ������
	QuadTree(QuadTree* pParent);
	~QuadTree();

	//����Ʈ�� ����
	bool Build(VertexPNT* pHMap);
	//�ﰢ���� �ε����� ����� ����� �ﰢ�� ������ ����
	int GetIndex(LPVOID pIB, VertexPNT* pHMap, FrustumCulling* pFrustum, float fLODRatio);
	//ī�޶�� ����� �߾� �������� �Ÿ�
	float GetDistance(D3DXVECTOR3* v1, D3DXVECTOR3* v2);
	//�Ÿ����� �������� LOD�� ���
	int GetLODLevel(VertexPNT* pHMap, D3DXVECTOR3* pCam, float fLODRatio);
};

