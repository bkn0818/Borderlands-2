#ifndef PLANE_H
#define PLANE_H

#pragma once
#include "resource.h"
#include "QuadTree.h"
#include "Picking.h"
#define MAX_TEXTURE 50

struct MYINDEX
{
	WORD a, b, c;
};

struct CUSTOMCIRCLE
{
	D3DXVECTOR3 Pos;

	enum { FVF = D3DFVF_XYZ, };
};

class Plane
{
private:
	int m_ixDIB; //DIB�� ���� �ȼ�
	int m_izDIB; //DIB�� ���� �ȼ�
	int m_iyDIB; //DIB�� �ִ� ���̰�

	int m_iWidth; //�� ����
	int m_iDepth; //�� ����

	int m_iTriangles; //����� �ﰢ���� ����

	float m_fLODRatio; //LODó���ÿ� ���� ��

	float m_fTexScale; //�ؽ��� ���� ������

	int m_iSplateIdx; //���õ� ���÷��� �ε���
	int m_iSplateTex[MAX_TEXTURE]; //���÷��� �ؽ��� �ε���
	int m_iBaseTex; //�⺻�ؽ��� �ε���

	QuadTree* m_pQuadTree; //���� Ʈ�� ��ü�� ������

	D3DXVECTOR3 m_vScale; //������ ����

	VertexPNT* m_pHeightMap; //���̸��� ���� �迭

	LPDIRECT3DTEXTURE9 m_pTex[MAX_TEXTURE]; //�ؽ���
	LPDIRECT3DTEXTURE9 m_pAlpha[MAX_TEXTURE]; //���ĸ�

	LPDIRECT3DVERTEXBUFFER9 m_pVB; //���� ����
	LPDIRECT3DVERTEXBUFFER9 m_pVBPicking; //��ŷ �� ���� ����
	LPDIRECT3DINDEXBUFFER9 m_pIB; //�ε��� ����

	int m_iPickingvalue; //���� ��Ʈ���� ��
	int m_iUpdown; //������ �ø��� ������ �� ���ϴ� ����

	int iBigCircle; //ū�� ũ��
	int iSmallCircle; //������ ũ��

	int m_iBigValue; //ū���� ������
	int m_iSmallValue; //�������� ������

	//���÷��� �ε��� ����
	int m_iTexPosX;
	int m_iTexPosY;

	//���� ��ü�� ����� �ؽ��ĸ� �о���δ�
	HRESULT LoadTexture(LPSTR lpTexFilename);

	//BMP������ ��� ���̸� ����
	HRESULT BuildHeightMap(char* lpFilename);

	//BMPũ�⿡ ���� ���� Ʈ�� ����
	HRESULT BuildQuadTree();

	//����, �ε��� ���۸� ����
	HRESULT InitVIB();


public:
	Plane();
	~Plane();

	HRESULT Render(); //ȭ�鿡 ���� ���
	HRESULT Destroy();

	//���� ��ü �ʱ�ȭ
	HRESULT InitPlane(D3DXVECTOR3* pScale, float fLODRatio, char* lpBMPFileName, LPSTR lpTexFileName);

	HRESULT DrawMesh(FrustumCulling* pFrustum, Picking* pRay, bool sFlag);
	DWORD ReturnZH();

	int Log2(int n);//2�� ������ �ϴ� ���� n�� �αװ� ���
	int Pow2(int n);//2^n �� ���

	LPBYTE DIBXYInv(LPBYTE lpDIB, int x, int z);
	int PalSize(LPBYTE lpDIB);
	int ALIGN4B(int n);

	LPBYTE DibLoadHandle(char* lpFileName);
	void DibDeleteHandle(LPBYTE lpSrc);

	//���� ��ŷ
	bool SearchPicking(Picking* pRay, MYINDEX* pIdx);

	//���� ������ ���̸� ���� ��������
	D3DXVECTOR3 GetHeightMap(int row, int col);

	//��ġ������ ���� ���̰� ���
	float GetHeightMapY(float x, float z);
	//��ġ������ ���̰� ����
	void SetHeightMapY(int row, int col, int y);
	//x, z������ ���̸� �ε��� ���
	void GetHeightMapIdx(float x, float z, int& iRow, int& iCol);
	//��������
	float LinearInterpolation(float a, float b, float T);

	//��ŷó��
	bool PickingMap(Picking* pRay, bool buttonFlag);
	//�� ���� ���� �缳��(������ ����)
	bool MakeVBMap(MYINDEX* pIdx);
	//��ŷ �� ���� ����
	bool MakeVBPicking(Picking* pRay);
	//�� �׸���
	HRESULT RenderCircle(Picking* pRay);
	//�⺻ �ؽ�ó ����
	void SetBaseTexture();
	//�����ؽ�ó �ɼ� ����
	void SetAlphaTexture();
	//�ؽ�ó ���÷���
	void RenderSplatting();
	//�����ؽ�ó ����
	bool CreateAlpha();
	//�����ؽ�ó �׸���
	bool DrawAlpha(Picking* pRay, int Idx);
	//��ŷ �� ���ϱ�
	void SetPickingValue(int x);
	//��ŷ �ø��� ������
	void SetPickingUpDown(int x);
	//��ŷ �� ũ�� ����
	void SetPickingBig(int x);
	void SetPickingSmall(int x);

	//������ ��ȯ
	int ReturnPickingValue();
	//��ŷ�� ũ�� ��ȯ
	int ReturnPickingBig();
	int ReturnPickingSmall();

	//��ŷ�� ������ ��ȯ
	int ReturnBigValue();
	int ReturnSmallValue();
	
	//��ŷ�� ������ ����
	void SetBigValue(int x);
	void SetSmallValue(int x);
};

#endif