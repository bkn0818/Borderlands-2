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

	//�׼��� ���� �׼ǽ�����
	ActionSequence* actionSeq = new ActionSequence;

	//�����̼� ��Ʈ����
	D3DXMATRIX matRotation;
	//Y������ 60�� ȸ����ų ��Ʈ����
	D3DXMatrixRotationY(&matRotation, D3DX_PI / 3.0f);

	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 0);
	//X������ 10��ŭ �̵���ų ����
	D3DXVECTOR3 p(10, 0, 0);

	for (int i = 0; i < 6; i++)
	{
		//6���� �׼� ����
		ActionMove* actionMove = new ActionMove;

		vertexs.push_back(VertexPC(p, c));
		actionMove->SetStartPos(p);

		//p�� 60�� ȸ����Ŵ
		D3DXVec3TransformCoord(&p, &p, &matRotation);

		vertexs.push_back(VertexPC(p, c));
		//Ÿ�� ������
		actionMove->SetTargetPos(p);
		//1�ʵ���
		actionMove->SetActionTime(1);
		//Ÿ�� = ť��
		actionMove->SetTargetObject(cube);
		//�׼ǽ������� ����׼� �߰�
		actionSeq->AddAction(actionMove);
	}

	//�ݺ� �׼Ǹ���Ʈ
	ActionRepeat* actionRepeat = new ActionRepeat;
	//�׼ǽ������� ��������Ʈ ����
	actionSeq->SetActionDelegate(actionRepeat);
	//�׼Ǹ���Ʈ�� �׼ǽ������� ����
	actionRepeat->SetAction(actionSeq);

	//ť�꿡 �ݺ��׼��� �ְ�
	cube->SetAction(actionRepeat);
	//�׼� ����
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