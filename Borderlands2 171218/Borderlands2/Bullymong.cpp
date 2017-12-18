#include "stdafx.h"
#include "Bullymong.h"


Bullymong::Bullymong()
{
}

Bullymong::~Bullymong()
{
}

HRESULT Bullymong::Init(D3DXVECTOR3 position)
{
	pos = position;

	sm = g_pSkinnedMeshManager->GetSkinnedMesh("zealot", "bullymong.X");
	sm->SetPosition(pos);
	curAnimSet = BMONG_WALK;
	sm->SetAnimationIndex(curAnimSet);

	state = STATE_WALK;
	hp = 586;

	// 충돌처리용 bounding Sphere들===========================
	SetBoundingSphere(range[RANGE_HEAD], 0.5f, 255, 0, 0);
	SetBoundingSphere(range[RANGE_ATTACK], 1.0f, 255, 0, 0);
	SetBoundingSphere(range[RANGE_RECOG], 8.0f, 0, 255, 0);
	SetBoundingSphere(range[RANGE_ASSAULT], 0.4f, 0, 0, 255);
	// =======================================================

	showRange = haveTarget = false;

	passedActionTime = 0.0f;
	moveInterval = 0.5f;

	//	animFrame[SLEEP_END] = animFrame[SLEEP_ING] = 31;
	//	animFrame[SLEEP_START] = animFrame[IDLE] = 101;

	return S_OK;
}

void Bullymong::Update(D3DXVECTOR3 position)
{
	Collision(position);
//	Move();


}

void Bullymong::Render()
{
	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMT);
	sm->UpdateAndRender();

	// bounding Sphere========================================
	D3DXMATRIXA16 matWorld;

	D3DXMatrixTranslation(&matWorld,
		pos.x,
		pos.y + 5,
		pos.z - 3);

	D3DDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	D3DDEVICE->SetMaterial(&range[RANGE_HEAD].material);

	range[RANGE_HEAD].sphereMesh->DrawSubset(0);

	//	for each(auto p in range)
	//	{
	//		p.sphereMesh->DrawSubset(0);
	//	}

	// =======================================================

}

void Bullymong::Release()
{}

void Bullymong::Assaulted(POINT mouse)
{
	//2D상의 점이
	//3D상의 구체와 충돌했을 때 
	//hp가 깎인다.

}

void Bullymong::Move(D3DXVECTOR3 playerPos)
{
	//	D3DXMATRIXA16				worldMT;
	//	std::vector<D3DXVECTOR3>	posList;			// 이동할 위치들
	//	D3DXVECTOR3					prevPos;
	//	D3DXVECTOR3					targetPos;			// 최종 위치
	//	float						moveInterval;		// 이동 간격
	//	float						passedActionTime;
	// random 돌려서 0 walk 1 Idle 상태로
	// walk라면
	// 선형 보간으로 prevPos = nowPos -> pos -> pos -> nextPos = targetPos

	//	D3DXMATRIXA16 matR;
	//	D3DXMatrixRotationY(&matR, D3DX_PI / 3.0f);

	//	D3DXVECTOR3 tempPos = startPos = pos;````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````
	//	D3DXVec3TransformCoord(&tempPos, &tempPos, &matR);
	prevPos = pos;

	if (haveTarget)
		targetPos = D3DXVECTOR3(playerPos.x, playerPos.y, playerPos.z - 1.0f);	// 플레이어 코 앞에 
	else
		targetPos = D3DXVECTOR3(pos.x + rand() % 2, pos.y, pos.z + rand() % 2);	// 랜덤 위치 

	passedActionTime += g_pTimeManager->GetDeltaTime();

	if (passedActionTime < moveInterval)
	{
		float t = passedActionTime / moveInterval;	// 텀 
		D3DXVECTOR3 p;
		D3DXVec3Lerp(&p, &prevPos, &targetPos, t); // startPosition에서 targetPosition으로 선형보간 
		posList.push_back(p);
	}
	else
	{
	}


	if (rand() % 2 == 0)
		curAnimSet = BMONG_WALK;
	else
		curAnimSet = BMONG_IDLE;

	sm->SetAnimationIndex(curAnimSet);

}

void Bullymong::Collision(D3DXVECTOR3 position)
{
	haveTarget = false;
	// player sphere의 radius 받아올 필요 있음. 
	if (CollisionSphere(position, 1.0f, pos, 5.0f))
	{
		haveTarget = true;
	}

	SetState(curAnimSet);
}

void Bullymong::SetState(int curAnim)
{
	if (haveTarget)
	{
		state = STATE_ATTACK;
		curAnimSet = BMONG_THROWROCK;
	}
	else
	{
		state = STATE_WALK;
		curAnimSet = BMONG_WALK;
	}

	sm->SetAnimationIndex(curAnimSet);
}

// Bounding Sphere show 여부 
void Bullymong::ShowRange()
{
	if (showRange == false)
		showRange = true;
	else
		showRange = false;
}

void Bullymong::SetAnim(int n)
{
	sm->SetAnimationIndex(n);
}