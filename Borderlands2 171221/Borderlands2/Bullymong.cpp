#include "stdafx.h"
#include "Bullymong.h"

// HeightMap
//       x
//       ^ +
//       | 
// z <---- - 
// +     -
Bullymong::Bullymong()
{
}

Bullymong::~Bullymong()
{
}

HRESULT Bullymong::Init(D3DXVECTOR3 position)
{
	pos = position;

	sm = g_pSkinnedMeshManager->GetSkinnedMesh("./XFile/", "bullymong.X");
	sm->SetMoving(false);
	sm->SetPosition(pos);
	curAnimSet = BMONG_WALK;
	sm->SetAnimationIndex(curAnimSet);

	state = STATE_WALK;
	hp = HPMAX_BMONG;

	// 충돌처리용 bounding Sphere들===========================
	SetBoundingSphere(range[RANGE_HEAD], 2.0f, 255, 0, 0);
	SetBoundingSphere(range[RANGE_ATTACK], 1.0f, 255, 0, 0);
	SetBoundingSphere(range[RANGE_RECOG], 20.0f, 255, 0, 0);
	SetBoundingSphere(range[RANGE_ASSAULT], 0.4f, 0, 0, 255);
	// =======================================================

	// 던지는 돌
	D3DXCreateSphere(D3DDEVICE, 0.2f, 20, 20, &rockObj.mesh, NULL);
	rockObj.gravity = 0.f;
	rockObj.passedTime = 0.f;
	rockObj.throwTime = 5.f;

	isRecog = isMoveStart = false;
	showRange = false;

	passedActionTime = 0.0f;
	moveInterval = 0.1f;

	sm->RotateY(-30);

	isThrow = false;
	throwStart = false;
	count = 0;

	return S_OK;
}

void Bullymong::Update(iMap* obj, D3DXVECTOR3 playerPosition, SphereInfo* playerSphere)
{
	Collision(playerPosition, playerSphere);

	// HeightMap 좌표 처리=============================
	D3DXVECTOR3 temp = pos;

	if (obj && obj->GetHeight(temp.x, temp.y, temp.z))
	{
		pos = temp;
	}

	sm->SetPosition(pos);
	// ================================================

}

void Bullymong::Render()
{
	// 거리 테스트 
	LPD3DXFONT font = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc = { 400, 0, 600, 50 };
	char str[1024];
	sprintf(str, "dis:%.3f, E(%.3f, %.3f, %3.f) P(%.3f, %.3f, %.3f) curAnim: %d", distance, pos.x, pos.y, pos.z, px, py, pz, curAnimSet);
	font->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 0));

	LPD3DXFONT font2 = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc2 = { 400, 60, 600, 100};
	char str2[1024];
	sprintf(str2, "R(%.3f, %.3f, %.3f) R_TAR(%.3f, %.3f, %.3f) Angle: %.3f", rockObj.pos.x, rockObj.pos.y, rockObj.pos.z, rockObj.targetPos.x, rockObj.targetPos.y, rockObj.targetPos.z, rockObj.angle);
	font2->DrawTextA(nullptr, str2, strlen(str2), &rc2, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));
	
	sm->UpdateAndRender();

	// bounding Sphere========================================
	D3DXMATRIXA16 matWorld;

	D3DXMatrixTranslation(&matWorld,
		pos.x,
		pos.y,
		pos.z);

	D3DDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DDEVICE->SetMaterial(&range[RANGE_RECOG].material);

	range[RANGE_RECOG].sphereMesh->DrawSubset(0);

	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // 원래대로 

	//	for each(auto p in range)
	//	{
	//		p.sphereMesh->DrawSubset(0);
	//	}

	// =======================================================

	// throwObj===============================================
	D3DXMATRIXA16 rockWorld;

	D3DXMatrixTranslation(&rockWorld,
		rockObj.pos.x,
		rockObj.pos.y,
		rockObj.pos.z);

	rockWorld = rockWorld;

	D3DMATERIAL9 throwRock;
	ZeroMemory(&throwRock, sizeof(D3DMATERIAL9));
	throwRock.Diffuse.r = throwRock.Ambient.r = 1.f;
	throwRock.Diffuse.g = throwRock.Ambient.g = 1.f;
	throwRock.Diffuse.b = throwRock.Ambient.b = 1.f;
	throwRock.Diffuse.a = throwRock.Ambient.a = 1.f;
	D3DDEVICE->SetTransform(D3DTS_WORLD, &rockWorld);
	D3DDEVICE->SetMaterial(&throwRock);
	rockObj.mesh->DrawSubset(0);

	// rock 확인용====================================
	D3DXMATRIXA16 testMT;
	D3DXMatrixIdentity(&testMT);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &testMT);
	if (rockObj.posList.size() > 0)
	{
		D3DDEVICE->SetFVF(VertexPC::FVF);
		D3DDEVICE->DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			rockObj.posList.size() / 2,
			&rockObj.posList[0],
			sizeof(VertexPC)
		);
	}
}

void Bullymong::Release()
{}

void Bullymong::Assaulted(POINT mouse)
{
	//2D상의 점이
	//3D상의 구체와 충돌했을 때 
	//hp가 깎인다.

}

void Bullymong::Collision(D3DXVECTOR3 position, SphereInfo* playerSphere)
{ 
	if (CollisionSphere(pos, range[RANGE_RECOG].sphereInfo.fRadius, position, playerSphere->fRadius))
	{
		isRecog = true;	// 인식했다 
	}

	// 데이터 확인용=================================
	px = position.x;
	py = position.y;
	pz = position.z;
	// ==============================================

	Move(position);
	Attack();
}

void Bullymong::Move(D3DXVECTOR3 playerPosition)
{
	srand(time(NULL));
	int rNum = rand() % 2;
	bool isMoveEnd = false; // 다 움직였는가 

	// 1. 인식O: THROWROCK 혹은 RUN 혹은 PUNCH
	if (isRecog)
	{
		// hp가 30%이상이고 플레이어와의 간격이 ~이상일 때 돌던지기 
		if (hp > HPMAX_BMONG * 0.3 && distance >= 10.f)
		{
			if (curAnimSet != BMONG_THROWROCK)
			{
				curAnimSet = BMONG_THROWROCK;
				sm->SetAnimationIndex(curAnimSet);
			}
			else if (curAnimSet == BMONG_THROWROCK)
			{
				if (!isThrow)
				{
					isThrow = true; // 돌 움직이기 시작 
					rockObj.pos = D3DXVECTOR3(pos.x - 1.f, pos.y+5, pos.z+3);
					rockObj.targetPos = playerPosition;
					D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
					rockObj.posList.push_back(VertexPC(rockObj.pos, color));
					throwStart = true;

					rockObj.nowY = rockObj.pos.y;
					rockObj.lerpPos = rockObj.pos;

					//          targetPos
					//         /|   z 
					//        / |   |
					//   a   /  |   |   cos = b/a;
					//      /   |   |   a = D3DXVec3Length(targetPos - pos) 
					//     /    |   |   b = targetPos.x - pos.x 
					//    /_____|   |   acos(cos) * 180 / PI 
					// pos   b      |
					//  ---------------x
					float cos, a, b;
					D3DXVECTOR3 temp = rockObj.targetPos - rockObj.pos;
					a = D3DXVec3Length(&temp);
					b = rockObj.pos.x - rockObj.targetPos.x;
					cos = b / a;
					rockObj.angle = acosf(cos) * (180 / PI);
				}
			}
		}
		// 플레이어와의 간격이 ~만큼 넓어졌을 때 인식 해제 
		if (distance >= range[RANGE_RECOG].sphereInfo.fRadius * 2)
		{
			isRecog = false;
		}
	}
	// 1. 인식X: IDLE 혹은 WALK 
	else
	{
		// 2. WALK도 IDLE도 아님: 둘 중 하나로  
		if (curAnimSet != BMONG_WALK && curAnimSet != BMONG_IDLE)
		{
			if (!rNum)	curAnimSet = BMONG_WALK;
			else		curAnimSet = BMONG_IDLE;
			sm->SetAnimationIndex(curAnimSet);
		}
	}
		/*
		// 2. WALK도 IDLE도 아님: 둘 중 하나로  
		if (curAnim != BMONG_WALK && curAnim != BMONG_IDLE)
		{
			isMoveEnd = true;
		}
		else
		{
			passedActionTime += g_pTimeManager->GetDeltaTime();

			// 2. 지금 WALK: 목표에 도달할 때까지 이동
			if (curAnim == BMONG_WALK)
			{
				// 3. 움직이기 시작O: 선형보간
				if (isMoveStart)
				{
					// 4. 지금 지점과 목표 지점간 거리가 같음: 그만 이동하자
					D3DXVECTOR3 dis = targetPos - pos;

					if (D3DXVec3Length(&dis) <= EPSILON)
					{
						isMoveEnd = true;	//도착했다 					
					}
					// 4. 지금 지점과 목표 지점간 거리가 다름: 이동하자
					else
					{
						// 5. 이동 텀마다 선형보간 
						if (passedActionTime < moveInterval)
						{
							float t = passedActionTime / moveInterval;	// 텀 
							D3DXVECTOR3 p;
							D3DXVec3Lerp(&p, &pos, &targetPos, t); // 현재position에서 targetPosition으로 선형보간 
							pos = p;
						}
					}
				}
				// 3. 움직이기 시작X: 자 이제 시작이야 + 목표지점 설정  
				else
				{
					isMoveStart = true;
					int a = rand() % 15;
					int b = rand() % 10;
					targetPos = D3DXVECTOR3(pos.x + a - b, pos.y, pos.z + a - b);	// 랜덤 위치
				}
			}
			// 2. 지금 IDLE: 시간 됐으면 초기화 
			else if (curAnim == BMONG_IDLE)
			{
				if (passedActionTime > moveInterval)
					isMoveEnd = true;
			}
		}
		// 움직임이 끝났으니 초기화 
		if (isMoveEnd)
		{
			isMoveStart = false; // 목표지점은 없어졌다 
			passedActionTime = 0.f;
			rNum = rand() % 2;

			if (!rNum)		curAnim = BMONG_WALK;
			else if (rNum)	curAnim = BMONG_IDLE;
			sm->SetAnimationIndex(curAnim);
		}
	}
	*/
}

void Bullymong::Attack()
{
	if (curAnimSet == BMONG_THROWROCK)
	{
		if (isThrow)
		{
//			if(!throwStart)
//				count++;
//
//			if (count == 80)
//			{
//				throwStart = true;
//				count = 0;
//			}

			// 던지는 타이밍 조정 필요. 
			if (throwStart)
			{
				rockObj.passedTime += g_pTimeManager->GetDeltaTime();

				if (rockObj.passedTime < rockObj.throwTime)
				{
					float t = rockObj.passedTime / rockObj.throwTime;

					D3DXVECTOR3 p;
					D3DXVec3Lerp(&p, &rockObj.lerpPos, &rockObj.targetPos, t); // 현재position에서 targetPosition으로 선형보간 

					rockObj.lerpPos = p;
					rockObj.nowY += 0.1f - rockObj.gravity;
					rockObj.gravity += 0.01f;

					rockObj.pos = D3DXVECTOR3(rockObj.lerpPos.x, rockObj.nowY, rockObj.lerpPos.z);

					D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
					rockObj.posList.push_back(VertexPC(rockObj.pos, color));

					if (rockObj.nowY < 0)
					{
						isThrow = false;
						rockObj.gravity = 0.f;
						rockObj.passedTime = 0.f;
						rockObj.posList.clear();
						throwStart = false;
					}
				}
			}
		}
			/*
			if (rockObj.pos.y < rockObj.targetPos.y)
			{
				rockObj.pos = D3DXVECTOR3(pos.x - 1.f, pos.y, pos.z);
				rockObj.gravity = 0.f;
				isThrow = false;
				rockObj.posList.clear();
			}

			rockObj.pos.x -= 0.2f;
			rockObj.pos.y += 0.2f - rockObj.gravity;
			rockObj.gravity += 0.005f;

			float dis = rockObj.targetPos.z - rockObj.pos.z;

			if (dis >= EPSILON)
			{
				if (rockObj.pos.z > rockObj.targetPos.z)
					rockObj.pos.z -= 0.05f;
				else if (rockObj.pos.z <= rockObj.targetPos.z)
					rockObj.pos.z += 0.05f;
			}

			D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
			rockObj.posList.push_back(VertexPC(rockObj.pos, color));
			*/
	}

	// targetPos와 playerPosition의 x 혹은 z좌표에 차이가 생겼을 경우
	// pos는 최대한 playerPosition에 맞춰 z든 x든 + 한다 

	//	LPD3DXFRAME testL = D3DXFrameFind((D3DXFRAME*)_player.skinnedMesh->GetRoot(), "FX_Rock_R1");
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