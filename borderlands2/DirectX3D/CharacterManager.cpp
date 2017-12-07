#include "stdafx.h"
#include "CharacterManager.h"

CharacterManager::CharacterManager()
	:texture(nullptr)
{
}


CharacterManager::~CharacterManager()
{
}

void CharacterManager::SetTextureVertex(std::vector<D3DXVECTOR2>& UVList, std::vector<D3DXVECTOR2> UVIndex)
{
	UVList.push_back(D3DXVECTOR2(UVIndex[0].x / 16.0f, UVIndex[0].y / 8.0f));
	UVList.push_back(D3DXVECTOR2(UVIndex[1].x / 16.0f, UVIndex[1].y / 8.0f));
	UVList.push_back(D3DXVECTOR2(UVIndex[2].x / 16.0f, UVIndex[2].y / 8.0f));

	UVList.push_back(D3DXVECTOR2(UVIndex[0].x / 16.0f, UVIndex[0].y / 8.0f));
	UVList.push_back(D3DXVECTOR2(UVIndex[2].x / 16.0f, UVIndex[2].y / 8.0f));
	UVList.push_back(D3DXVECTOR2(UVIndex[3].x / 16.0f, UVIndex[3].y / 8.0f));
}

void CharacterManager::Init()
{
	texture = g_pTextureManager->GetTexture("Textures/batman.png");


	D3DXMATRIXA16 scaleMatrix, translationMatrix, relativeResult;
	D3DXMatrixIdentity(&translationMatrix);
	std::vector<D3DXVECTOR2> UVList;
	std::vector<D3DXVECTOR2> UVIndexList;


	// relative 는 scale, animation pivot을 위해 자리 이동을 초기화
	// pivot 애니메이션 연산 후
	// SetLocalTranslation을 이용해 다시 원래 자리로 이동한다.. 


	//큐브맨 몸통(rootCube)
	UVIndexList.push_back(D3DXVECTOR2(8, 8));
	UVIndexList.push_back(D3DXVECTOR2(8, 5));
	UVIndexList.push_back(D3DXVECTOR2(10, 5));
	UVIndexList.push_back(D3DXVECTOR2(10, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(5, 8));
	UVIndexList.push_back(D3DXVECTOR2(5, 5));
	UVIndexList.push_back(D3DXVECTOR2(7, 5));
	UVIndexList.push_back(D3DXVECTOR2(7, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(7, 8));
	UVIndexList.push_back(D3DXVECTOR2(7, 5));
	UVIndexList.push_back(D3DXVECTOR2(8, 5));
	UVIndexList.push_back(D3DXVECTOR2(8, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(4, 8));
	UVIndexList.push_back(D3DXVECTOR2(4, 5));
	UVIndexList.push_back(D3DXVECTOR2(5, 5));
	UVIndexList.push_back(D3DXVECTOR2(5, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(5, 5));
	UVIndexList.push_back(D3DXVECTOR2(5, 4));
	UVIndexList.push_back(D3DXVECTOR2(7, 4));
	UVIndexList.push_back(D3DXVECTOR2(7, 5));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(7, 4));
	UVIndexList.push_back(D3DXVECTOR2(7, 5));
	UVIndexList.push_back(D3DXVECTOR2(9, 5));
	UVIndexList.push_back(D3DXVECTOR2(9, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	//rootCube
	D3DXMatrixScaling(&scaleMatrix, 1.0f, 1.5f, 0.5f);
	relativeResult = scaleMatrix * translationMatrix;
	//rootCube.InitWithTexture(relativeResult, UVList);
	UVList.clear();

	//큐브맨 머리
	UVIndexList.push_back(D3DXVECTOR2(6, 4));
	UVIndexList.push_back(D3DXVECTOR2(6, 2));
	UVIndexList.push_back(D3DXVECTOR2(8, 2));
	UVIndexList.push_back(D3DXVECTOR2(8, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(2, 4));
	UVIndexList.push_back(D3DXVECTOR2(2, 2));
	UVIndexList.push_back(D3DXVECTOR2(4, 2));
	UVIndexList.push_back(D3DXVECTOR2(4, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(4, 4));
	UVIndexList.push_back(D3DXVECTOR2(4, 2));
	UVIndexList.push_back(D3DXVECTOR2(6, 2));
	UVIndexList.push_back(D3DXVECTOR2(6, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(0, 4));
	UVIndexList.push_back(D3DXVECTOR2(0, 2));
	UVIndexList.push_back(D3DXVECTOR2(2, 2));
	UVIndexList.push_back(D3DXVECTOR2(2, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(2, 2));
	UVIndexList.push_back(D3DXVECTOR2(2, 0));
	UVIndexList.push_back(D3DXVECTOR2(4, 0));
	UVIndexList.push_back(D3DXVECTOR2(4, 2));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(4, 0));
	UVIndexList.push_back(D3DXVECTOR2(4, 2));
	UVIndexList.push_back(D3DXVECTOR2(6, 2));
	UVIndexList.push_back(D3DXVECTOR2(6, 0));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	//head
	CubeCtrl* head = new CubeCtrl;
	D3DXMatrixIdentity(&relativeResult);
	//head->InitWithTexture(relativeResult, UVList);
	head->SetLocalTranslation(D3DXVECTOR3(0.0f, 2.5f, 0.0f));
	rootCube.AddChild(head);
	UVList.clear();

	//큐브맨 왼쪽 팔
	UVIndexList.push_back(D3DXVECTOR2(11, 8));
	UVIndexList.push_back(D3DXVECTOR2(11, 5));
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	UVIndexList.push_back(D3DXVECTOR2(12, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(13, 8));
	UVIndexList.push_back(D3DXVECTOR2(13, 5));
	UVIndexList.push_back(D3DXVECTOR2(14, 5));
	UVIndexList.push_back(D3DXVECTOR2(14, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(10, 8));
	UVIndexList.push_back(D3DXVECTOR2(10, 5));
	UVIndexList.push_back(D3DXVECTOR2(11, 5));
	UVIndexList.push_back(D3DXVECTOR2(11, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(12, 8));
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(11, 5));
	UVIndexList.push_back(D3DXVECTOR2(11, 4));
	UVIndexList.push_back(D3DXVECTOR2(12, 4));
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(12, 4));
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	CubeCtrl* leftArm = new CubeCtrl;
	D3DXMatrixScaling(&scaleMatrix, 0.5f, 1.5f, 0.5f);
	D3DXMatrixTranslation(&translationMatrix, 0.0f, -1.5f, 0.0f);
	relativeResult = scaleMatrix * translationMatrix;
	//leftArm->InitWithTexture(relativeResult, UVList);
	leftArm->SetLocalTranslation(D3DXVECTOR3(-1.5f, 1.5f, 0.0f));
	leftArm->SetAngleSpeed(0.1f);
	rootCube.AddChild(leftArm);
	UVList.clear();

	//큐브맨 오른쪽 팔
	UVIndexList.push_back(D3DXVECTOR2(12, 8));
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	UVIndexList.push_back(D3DXVECTOR2(11, 5));
	UVIndexList.push_back(D3DXVECTOR2(11, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(14, 8));
	UVIndexList.push_back(D3DXVECTOR2(14, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(12, 8));
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 5));
	UVIndexList.push_back(D3DXVECTOR2(13, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(10, 8));
	UVIndexList.push_back(D3DXVECTOR2(10, 5));
	UVIndexList.push_back(D3DXVECTOR2(11, 5));
	UVIndexList.push_back(D3DXVECTOR2(11, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	UVIndexList.push_back(D3DXVECTOR2(12, 4));
	UVIndexList.push_back(D3DXVECTOR2(11, 4));
	UVIndexList.push_back(D3DXVECTOR2(11, 5));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(13, 4));
	UVIndexList.push_back(D3DXVECTOR2(13, 5));
	UVIndexList.push_back(D3DXVECTOR2(12, 5));
	UVIndexList.push_back(D3DXVECTOR2(12, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	CubeCtrl* rightArm = new CubeCtrl;
	D3DXMatrixScaling(&scaleMatrix, 0.5f, 1.5f, 0.5f);
	D3DXMatrixTranslation(&translationMatrix, 0.0f, -1.5f, 0.0f);
	relativeResult = scaleMatrix * translationMatrix;
	//rightArm->InitWithTexture(relativeResult, UVList);
	rightArm->SetLocalTranslation(D3DXVECTOR3(1.5f, 1.5f, 0.0f));
	rightArm->SetAngleSpeed(-0.1f);
	rootCube.AddChild(rightArm);
	UVList.clear();

	//큐브맨 왼쪽 다리
	UVIndexList.push_back(D3DXVECTOR2(3, 8));
	UVIndexList.push_back(D3DXVECTOR2(3, 5));
	UVIndexList.push_back(D3DXVECTOR2(4, 5));
	UVIndexList.push_back(D3DXVECTOR2(4, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(1, 8));
	UVIndexList.push_back(D3DXVECTOR2(1, 5));
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	UVIndexList.push_back(D3DXVECTOR2(2, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(2, 8));
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	UVIndexList.push_back(D3DXVECTOR2(3, 5));
	UVIndexList.push_back(D3DXVECTOR2(3, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(0, 8));
	UVIndexList.push_back(D3DXVECTOR2(0, 5));
	UVIndexList.push_back(D3DXVECTOR2(1, 5));
	UVIndexList.push_back(D3DXVECTOR2(1, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(1, 5));
	UVIndexList.push_back(D3DXVECTOR2(1, 4));
	UVIndexList.push_back(D3DXVECTOR2(2, 4));
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(2, 4));
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	UVIndexList.push_back(D3DXVECTOR2(3, 5));
	UVIndexList.push_back(D3DXVECTOR2(3, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	CubeCtrl* leftLeg = new CubeCtrl;
	D3DXMatrixScaling(&scaleMatrix, 0.5f, 1.5f, 0.5f);
	D3DXMatrixTranslation(&translationMatrix, 0.0f, -1.5f, 0.0f);
	relativeResult = scaleMatrix * translationMatrix;
	//leftLeg->InitWithTexture(relativeResult, UVList);
	leftLeg->SetLocalTranslation(D3DXVECTOR3(-0.5f, -1.5f, 0.0f));
	leftLeg->SetAngleSpeed(0.1f);
	rootCube.AddChild(leftLeg);
	UVList.clear();

	//큐브맨 오른쪽 다리
	UVIndexList.push_back(D3DXVECTOR2(4, 8));
	UVIndexList.push_back(D3DXVECTOR2(4, 5));
	UVIndexList.push_back(D3DXVECTOR2(3, 5));
	UVIndexList.push_back(D3DXVECTOR2(3, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(2, 8));
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	UVIndexList.push_back(D3DXVECTOR2(1, 5));
	UVIndexList.push_back(D3DXVECTOR2(1, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(1, 8));
	UVIndexList.push_back(D3DXVECTOR2(1, 5));
	UVIndexList.push_back(D3DXVECTOR2(0, 5));
	UVIndexList.push_back(D3DXVECTOR2(0, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(3, 8));
	UVIndexList.push_back(D3DXVECTOR2(3, 5));
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	UVIndexList.push_back(D3DXVECTOR2(2, 8));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	UVIndexList.push_back(D3DXVECTOR2(2, 4));
	UVIndexList.push_back(D3DXVECTOR2(1, 4));
	UVIndexList.push_back(D3DXVECTOR2(1, 5));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	UVIndexList.push_back(D3DXVECTOR2(3, 4));
	UVIndexList.push_back(D3DXVECTOR2(3, 5));
	UVIndexList.push_back(D3DXVECTOR2(2, 5));
	UVIndexList.push_back(D3DXVECTOR2(2, 4));
	SetTextureVertex(UVList, UVIndexList);
	UVIndexList.clear();
	CubeCtrl* rightLeg = new CubeCtrl;
	D3DXMatrixScaling(&scaleMatrix, 0.5f, 1.5f, 0.5f);
	D3DXMatrixTranslation(&translationMatrix, 0.0f, -1.5f, 0.0f);
	relativeResult = scaleMatrix * translationMatrix;
	//rightLeg->InitWithTexture(relativeResult, UVList);
	rightLeg->SetLocalTranslation(D3DXVECTOR3(0.5f, -1.5f, 0.0f));	
	rightLeg->SetAngleSpeed(-0.1f);
	rootCube.AddChild(rightLeg);
	UVList.clear();
}

void CharacterManager::Update(D3DXMATRIXA16 & parentWorldMatrix)
{
	//rootCube.Update(parentWorldMatrix);
}

void CharacterManager::Render()
{
	D3DDEVICE->SetTexture(0, texture);
	//rootCube.Render();
}