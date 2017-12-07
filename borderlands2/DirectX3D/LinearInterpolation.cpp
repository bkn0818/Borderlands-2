#include "stdafx.h"
#include "LinearInterpolation.h"


LinearInterpolation::LinearInterpolation()
{
}


LinearInterpolation::~LinearInterpolation()
{
}

void LinearInterpolation::Init()
{
	cube = new CubeCtrl;
	//cube->Init();

	//액션을 담을 액션시퀀스
	ActionSequence* actionSeq = new ActionSequence;

	//로테이션 매트릭스
	D3DXMATRIX matRotation;
	//Y축으로 60도 회전시킬 매트릭스
	D3DXMatrixRotationY(&matRotation, D3DX_PI / 3.0f);

	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 0);
	//X축으로 10만큼 이동시킬 벡터
	D3DXVECTOR3 p(10, 0, 0);

	for (int i = 0; i < 6; i++)
	{
		//6개의 액션 생성
		ActionMove* actionMove = new ActionMove;

		vertexs.push_back(VertexPC(p, c));
		actionMove->SetStartPos(p);

		//p를 60도 회전시킴
		D3DXVec3TransformCoord(&p, &p, &matRotation);

		vertexs.push_back(VertexPC(p, c));
		//타겟 포지션
		actionMove->SetTargetPos(p);
		//1초동안
		actionMove->SetActionTime(1);
		//타겟 = 큐브
		actionMove->SetTargetObject(cube);
		//액션시퀀스에 무브액션 추가
		actionSeq->AddAction(actionMove);
	}

	//반복 액션리피트
	ActionRepeat* actionRepeat = new ActionRepeat;
	//액션시퀀스에 델리게이트 세팅
	actionSeq->SetActionDelegate(actionRepeat);
	//액션리피트에 액션시퀀스를 넣음
	actionRepeat->SetAction(actionSeq);

	//큐브에 반복액션을 넣고
	cube->SetAction(actionRepeat);
	//액션 시작
	actionRepeat->Start();
}

void LinearInterpolation::Update()
{
	if (cube) cube->Update();
}

void LinearInterpolation::Render()
{
	D3DXMATRIX worldMat;
	D3DXMatrixIdentity(&worldMat);
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, false);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMat);
	D3DDEVICE->SetFVF(VertexPC::FVF);
	D3DDEVICE->DrawPrimitiveUP(D3DPT_LINELIST, vertexs.size() / 2, &vertexs[0], sizeof(VertexPC));
	//if (cube) cube->Render();
}