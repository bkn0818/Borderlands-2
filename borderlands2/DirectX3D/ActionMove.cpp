#include "stdafx.h"
#include "ActionMove.h"


ActionMove::ActionMove()
	:startPos(0, 0, 0), targetPos(0, 0, 0)
{
}


ActionMove::~ActionMove()
{
}

void ActionMove::Start()
{
	if (!targetObject) return;

	//Ÿ���� �������� ��ŸƮ ��������
	targetObject->SetPosition(startPos);
	passedActionTime = 0.0f;
}

void ActionMove::Update()
{
	if (!targetObject) return;

	passedActionTime += g_pTimeManager->GetDeltaTime();

	if (passedActionTime < actionTime)
	{
		float lerp = passedActionTime / actionTime;
		D3DXVECTOR3 p;
		D3DXVec3Lerp(&p, &startPos, &targetPos, lerp);
		targetObject->SetPosition(p);
	}
	else
	{
		if (actionDelegate)
		{
			actionDelegate->OnActionFinish(this);
		}
	}
}
