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
	int m_ixDIB; //DIB의 가로 픽셀
	int m_izDIB; //DIB의 세로 픽셀
	int m_iyDIB; //DIB의 최대 높이값

	int m_iWidth; //맵 넓이
	int m_iDepth; //맵 깊이

	int m_iTriangles; //출력할 삼각형의 개수

	float m_fLODRatio; //LOD처리시에 사용될 값

	float m_fTexScale; //텍스쳐 맵핑 스케일

	int m_iSplateIdx; //선택된 스플레팅 인덱스
	int m_iSplateTex[MAX_TEXTURE]; //스플레팅 텍스쳐 인덱스
	int m_iBaseTex; //기본텍스쳐 인덱스

	QuadTree* m_pQuadTree; //쿼드 트리 객체의 포인터

	D3DXVECTOR3 m_vScale; //스케일 벡터

	VertexPNT* m_pHeightMap; //높이맵의 정점 배열

	LPDIRECT3DTEXTURE9 m_pTex[MAX_TEXTURE]; //텍스쳐
	LPDIRECT3DTEXTURE9 m_pAlpha[MAX_TEXTURE]; //알파맵

	LPDIRECT3DVERTEXBUFFER9 m_pVB; //정점 버퍼
	LPDIRECT3DVERTEXBUFFER9 m_pVBPicking; //픽킹 원 정점 버퍼
	LPDIRECT3DINDEXBUFFER9 m_pIB; //인덱스 버퍼

	int m_iPickingvalue; //지형 컨트롤의 값
	int m_iUpdown; //지형을 올리고 내리는 걸 정하는 변수

	int iBigCircle; //큰원 크기
	int iSmallCircle; //작은원 크기

	int m_iBigValue; //큰원의 증가값
	int m_iSmallValue; //작은원의 증가값

	//스플래팅 인덱스 범위
	int m_iTexPosX;
	int m_iTexPosY;

	//지형 객체에 사용할 텍스쳐를 읽어들인다
	HRESULT LoadTexture(LPSTR lpTexFilename);

	//BMP파일을 열어서 높이맵 생성
	HRESULT BuildHeightMap(char* lpFilename);

	//BMP크기에 맞춰 쿼드 트리 생성
	HRESULT BuildQuadTree();

	//정점, 인덱스 버퍼를 생성
	HRESULT InitVIB();


public:
	Plane();
	~Plane();

	HRESULT Render(); //화면에 지형 출력
	HRESULT Destroy();

	//지형 객체 초기화
	HRESULT InitPlane(D3DXVECTOR3* pScale, float fLODRatio, char* lpBMPFileName, LPSTR lpTexFileName);

	HRESULT DrawMesh(FrustumCulling* pFrustum, Picking* pRay, bool sFlag);
	DWORD ReturnZH();

	int Log2(int n);//2를 밑으로 하는 숫자 n의 로그값 계산
	int Pow2(int n);//2^n 값 계산

	LPBYTE DIBXYInv(LPBYTE lpDIB, int x, int z);
	int PalSize(LPBYTE lpDIB);
	int ALIGN4B(int n);

	LPBYTE DibLoadHandle(char* lpFileName);
	void DibDeleteHandle(LPBYTE lpSrc);

	//지형 피킹
	bool SearchPicking(Picking* pRay, MYINDEX* pIdx);

	//열과 행으로 높이맵 정보 가져오기
	D3DXVECTOR3 GetHeightMap(int row, int col);

	//위치값으로 맵의 높이값 얻기
	float GetHeightMapY(float x, float z);
	//위치값으로 높이값 설정
	void SetHeightMapY(int row, int col, int y);
	//x, z값으로 높이맵 인덱스 얻기
	void GetHeightMapIdx(float x, float z, int& iRow, int& iCol);
	//선형보간
	float LinearInterpolation(float a, float b, float T);

	//피킹처리
	bool PickingMap(Picking* pRay, bool buttonFlag);
	//맵 정점 버퍼 재설정(높낮이 조절)
	bool MakeVBMap(MYINDEX* pIdx);
	//피킹 원 버퍼 설정
	bool MakeVBPicking(Picking* pRay);
	//원 그리기
	HRESULT RenderCircle(Picking* pRay);
	//기본 텍스처 설정
	void SetBaseTexture();
	//알파텍스처 옵션 설정
	void SetAlphaTexture();
	//텍스처 스플래팅
	void RenderSplatting();
	//알파텍스처 생성
	bool CreateAlpha();
	//알파텍스처 그리기
	bool DrawAlpha(Picking* pRay, int Idx);
	//피킹 값 정하기
	void SetPickingValue(int x);
	//피킹 올리고 내리기
	void SetPickingUpDown(int x);
	//피킹 원 크기 설정
	void SetPickingBig(int x);
	void SetPickingSmall(int x);

	//지형값 반환
	int ReturnPickingValue();
	//피킹원 크기 반환
	int ReturnPickingBig();
	int ReturnPickingSmall();

	//피킹원 증가값 반환
	int ReturnBigValue();
	int ReturnSmallValue();
	
	//피킹원 증가값 설정
	void SetBigValue(int x);
	void SetSmallValue(int x);
};

#endif