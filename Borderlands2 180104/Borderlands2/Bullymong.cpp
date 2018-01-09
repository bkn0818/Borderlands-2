#include "stdafx.h"
#include "Bullymong.h"


Bullymong::Bullymong()
{
}

Bullymong::~Bullymong()
{
}


//-----------------------------------------------------------------------------
// Bullymong::Init(D3DXVECTOR3 position)
// 설명: 초기화 
//-----------------------------------------------------------------------------
HRESULT Bullymong::Init(D3DXVECTOR3 position)
{
	pos = position;
	curAnim = BMONG_WALK;

	sm = g_pSkinnedMeshManager->GetEnemySkinnedMesh("./XFile/", "bullymong.X");
	sm->SetMoving(false);
	sm->SetPosition(pos);
	sm->SetAnimationIndex(curAnim);
	curAngle = -180;
	sm->RotateY(curAngle);					// 앞 보게끔

//	hp = HPMAX_BMONG;
	hp = 230;								// 깎인 상태 테스트용

	isRecog = false;
	showBSphere = false;
	passedTime = 0.f;
	actionTime = 40.f;
	isAttack = false;
	isIdle = false;
	throwStart = false;

	termAnimate = ANIMATETERM;
	termWalk = ANIMATETERM/2;

	// bounding Sphere============================================
	SetBoundingSphere(stBSphere[SP_HEAD], 2.0f, 255, 0, 0);
	SetBoundingSphere(stBSphere[SP_ASSAULT], 2.0f, 0, 0, 255);
	SetBoundingSphere(stBSphere[SP_RECOG], 20.0f, 255, 0, 0);
	SetBoundingSphere(stBSphere[SP_ATTACK], 10.f, 255, 0, 0);


	// 던지는 물체================================================
	D3DXCreateSphere(D3DDEVICE, 0.2f, 20, 20, &objRock.mesh, NULL);
	objRock.gravity = objRock.passedTime = 0.f;
	objRock.actionTime = 20.f;

	D3DXCOLOR color = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
	ZeroMemory(&objRock.material, sizeof(D3DMATERIAL9));
	objRock.material.Ambient = objRock.material.Diffuse = objRock.material.Specular = color;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Bullymong::Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer)
// 설명: imap, 플레이어의 좌표와 boundingSphere를 받아 업데이트 
//-----------------------------------------------------------------------------
void Bullymong::Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer)
{
	passedTime += g_pTimeManager->GetDeltaTime(); //0.016

	// test==========================================
	px = posPlayer.x;
	py = posPlayer.y;
	pz = posPlayer.z;
	// ==============================================

	IsRecogPlayer(posPlayer, spherePlayer);
	SetState(&posPlayer);

	//	Attack();

	SetHeightMap(imap);
}


//-----------------------------------------------------------------------------
// Bullymong::Render()
// 설명: draw
//-----------------------------------------------------------------------------
void Bullymong::Render()
{
	// 거리 테스트 
	LPD3DXFONT font = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc = { 400, 0, 600, 50 };
	char str[1024];
	sprintf(str, "dis:%.3f, E(%.3f, %.3f, %3.f) P(%.3f, %.3f, %.3f) curAnim: %d", distance, pos.x, pos.y, pos.z, px, py, pz, curAnim);
	font->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 0));

	LPD3DXFONT font2 = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc2 = { 400, 60, 600, 100 };
	char str2[1024];
	sprintf(str2, "R(%.3f, %.3f, %.3f) R_TAR(%.3f, %.3f, %.3f) Angle: %.3f", objRock.pos.x, objRock.pos.y, objRock.pos.z, objRock.posTarget.x, objRock.posTarget.y, objRock.posTarget.z, objRock.angle);
	font2->DrawTextA(nullptr, str2, strlen(str2), &rc2, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

	LPD3DXFONT font3 = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc3 = { 400, 130, 600, 150 };
	char str3[1024];
	sprintf(str3, "passedTime: %.3f angle: %.3f termAnimate: %.3f", passedTime, angle, termAnimate);
	font3->DrawTextA(nullptr, str3, strlen(str3), &rc3, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 0));

	LPD3DXFONT font4 = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc4 = { 400, 160, 600, 180 };
	char str4[1024];
	sprintf(str4, "atk: %d, move: %d, throw: %d, dirMove: %d", isAttack, isMove, throwStart, dirMove);
	font4->DrawTextA(nullptr, str4, strlen(str4), &rc4, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 0, 0));

	D3DXMATRIXA16 matWorld;

	D3DXMatrixTranslation(&matWorld,
		pos.x,
		pos.y,
		pos.z);

	D3DDEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	sm->UpdateAndRender();

	// bounding Sphere========================================
	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DDEVICE->SetMaterial(&stBSphere[SP_RECOG].material);

	stBSphere[SP_RECOG].sphereMesh->DrawSubset(0);

	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // 원래대로 

	//	for each(auto p in range)
	//	{
	//		p.sphereMesh->DrawSubset(0);
	//	}


	// throwObj===============================================
	D3DXMATRIXA16 matRock;

	D3DXMatrixTranslation(&matRock,
		objRock.pos.x,
		objRock.pos.y,
		objRock.pos.z);

	D3DDEVICE->SetTransform(D3DTS_WORLD, &matRock);
	D3DDEVICE->SetMaterial(&objRock.material);
	objRock.mesh->DrawSubset(0);

	/*
	// rock 확인용============================================
	D3DXMATRIXA16 matTest;
	D3DXMatrixIdentity(&matTest);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &matTest);
	D3DDEVICE->SetFVF(VertexPC::FVF);
	if (objRock.posList.size() > 0)
	{
		D3DDEVICE->DrawPrimitiveUP(
			D3DPT_TRIANGLELIST,
			objRock.posList.size() / 2,
			&objRock.posList[0],
			sizeof(VertexPC)
		);
	}
	*/
}


//-----------------------------------------------------------------------------
// Bullymong::Release()
// 설명: 
//-----------------------------------------------------------------------------
void Bullymong::Release()
{
}


//-----------------------------------------------------------------------------
// Bullymong::SetState(D3DXVECTOR3 posPlayer)
// 설명: 상태 변경 
//-----------------------------------------------------------------------------
void Bullymong::SetState(D3DXVECTOR3* posPlayer)
{
	// 플레이어와 enemy의 각도 ===============================
	//          targetPos
	//         /|    z 
	//        / |    |
	//   Hyp /  |    |   cos(angle) = Adj / Hyp;
	//      /   | Opp|   Hyp = D3DXVec3Length(targetPos - pos) 
	//     /    |    |   Opp = targetPos.x - pos.x 
	//    /_____|    |   acos(cos) * 180 / PI 
	// pos   Adj     |
	//  ---------------x
	float cos, hyp, adj;

	D3DXVECTOR3 vSub1;
	D3DXVec3Subtract(&vSub1, posPlayer, &pos);

	hyp = D3DXVec3Length(&vSub1);
	adj = posPlayer->x - pos.x;
	cos = adj / hyp;
	angle = acosf(cos) * (180 / PI);

	curAngle -= angle;
	sm->SetRotationY(curAngle);
	//========================================================

	// 근접 공격 범위 
	float closeAtkRange = stBSphere[SP_ATTACK].sphereInfo.fRadius;

	// 1. 인식O: THROWROCK / RUN / PUNCH
	if (isRecog) {
		// 2. 플레이어와의 간격이 근접 공격 범위 밖: THROWROCK / RUN
		if (distance > closeAtkRange) {
			// 3. HP 40% 이상: THROWROCK
			if (hp >= HPMAX_BMONG * 0.4) {
				if (!isAttack) {
					isAttack = true;
					curAnim = BMONG_THROWROCK;
				}
				else {
					// 4초마다 THROW / RUN
					if (termAnimate < 0 || curAnim == BMONG_IDLE) {
						termAnimate = ANIMATETERM;

						if (curAnim == BMONG_THROWROCK) {
							curAnim = BMONG_RUN_FRONT;
						}
						else if (curAnim == BMONG_RUN_FRONT || curAnim == BMONG_IDLE) {
							curAnim = BMONG_THROWROCK;
						}
						sm->SetAnimationIndex(curAnim);

						termWalk = ANIMATETERM;
						isMove = false;
						passedTime = 0.f;
						throwStart = false;
						objRock.gravity = objRock.passedTime = 0.f;
						objRock.posList.clear();	// 궤도 비우기 
					}
					else if (termAnimate >= 0) {
						if (curAnim == BMONG_THROWROCK)
							ThrowObj(posPlayer);
						else if (curAnim == BMONG_RUN_FRONT) 
							Move(posPlayer);
						else if (curAnim == BMONG_IDLE) {}
					}
				}
			}
			// 3. HP 40% 미만: RUN(근접공격하러)
			else if (hp < HPMAX_BMONG * 0.4) {
				if (curAnim != BMONG_RUN_FRONT) {
					isAttack = false;
					isMove = false;
					passedTime = 0.f;
					termWalk = ANIMATETERM;
					actionTime = 40.f;

					curAnim = BMONG_RUN_FRONT;
					sm->SetAnimationIndex(curAnim);
				}
				Move(posPlayer);
			}
		}
		// 2. 근접 공격 범위 내: PUNCH
		else if (distance <= closeAtkRange) {
			isAttack = true;
			if (posPlayer->z > pos.z && posPlayer->x <= pos.x) { // 3. 플레이어 위치에 따라 왼/우 펀치  
				curAnim = BMONG_PUNCH_RIGHT;
			}
			else if(posPlayer->z <= pos.z && posPlayer->x > pos.x){
				curAnim = BMONG_PUNCH_LEFT;
			}
			sm->SetAnimationIndex(curAnim);
		}
		// 2. 인식 범위 * 2이상: 인식 해제 
		else if (distance >= stBSphere[SP_RECOG].sphereInfo.fRadius * 2) {
			isRecog = false;
		}
	}
	// 1. 인식X: WALK / IDLE
	else {
	}

	termAnimate -= g_pTimeManager->GetDeltaTime();
}


//-----------------------------------------------------------------------------
// Bullymong::Move(D3DXVECTOR3 posPlayer)
// 설명: 이동 
//-----------------------------------------------------------------------------
void Bullymong::Move(D3DXVECTOR3* posPlayer)
{
	if (!isMove) {
		isMove = true;

		posTarget = *posPlayer;
	}
	else {
		D3DXVECTOR3 vSub;
		D3DXVec3Subtract(&vSub, &pos, &posTarget);
		float dis = D3DXVec3Length(&vSub);
		// throw 중일 때 좌우로 움직인다.
		if (isAttack) {
			if (dis > EPSILON) {
				if (posTarget.z > pos.z && posTarget.x <= pos.x) {
					pos.x -= MOVESPEED;
					pos.z += MOVESPEED;
				}
				else if (posTarget.z <= pos.z && posTarget.x > pos.x) {
					pos.x += MOVESPEED;
					pos.z -= MOVESPEED;
				}
			}
			else {
				curAnim = BMONG_IDLE;
				sm->SetAnimationIndex(curAnim);
			}
			if (termWalk < 0) {
				termWalk = ANIMATETERM;
				isMove = false;
				passedTime = 0.f;
			}
		}
		else {
			if (dis > EPSILON) {
				if (passedTime < actionTime) {
					float t = passedTime / actionTime;

					D3DXVECTOR3 p;
					D3DXVec3Lerp(&p, &pos, &posTarget, t);

					pos = p;
				}
			}
			else {
				termWalk = ANIMATETERM;
				actionTime = 40.f;
				isMove = false;
				passedTime = 0.f;
			}
		}
	}

	termWalk -= g_pTimeManager->GetDeltaTime();

}


//-----------------------------------------------------------------------------
// Bullymong::Attack(D3DXVECTOR3 posPlayer)
// 설명: 공격
//-----------------------------------------------------------------------------
void Bullymong::Attack()
{

}


//-----------------------------------------------------------------------------
// Bullymong::ThrowObj(D3DXVECTOR3 posPlayer)
// 설명: player의 좌표를 물체의 도착지점으로 설정해서 objRock을 계속 던진다 
//-----------------------------------------------------------------------------
void Bullymong::ThrowObj(D3DXVECTOR3* posPlayer)
{
	// 목표 설정
	if (!throwStart) {
		throwStart = true;
		objRock.pos = D3DXVECTOR3(pos.x - 1.f, pos.y + 5, pos.z + 3);
		objRock.posTarget = *posPlayer; // 플레이어에 닿게끔 
		objRock.nowY = objRock.pos.y;
		objRock.posLerp = objRock.pos;

		// 돌의 시작점과 플레이어 좌표 간 각도==================
		float c, a, b;

		D3DXVECTOR3 vSub;
		D3DXVec3Subtract(&vSub, &objRock.posTarget, &objRock.pos);

		a = D3DXVec3Length(&vSub);
		b = objRock.pos.x - objRock.posTarget.x;
		c = b / a;
		objRock.angle = acosf(c) * (180 / PI);

		// 돌 이동 궤도=========================================
		D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
		objRock.posList.push_back(VertexPC(objRock.pos, color));
	}
	// 던지기 
	else {
		objRock.passedTime += g_pTimeManager->GetDeltaTime();

		if (objRock.passedTime < objRock.actionTime) {
			float t = objRock.passedTime / objRock.actionTime;

			D3DXVECTOR3 p;
			D3DXVec3Lerp(&p, &objRock.posLerp, &objRock.posTarget, t);

			objRock.posLerp = p;
			objRock.nowY += THROWSPEED - objRock.gravity;
			objRock.gravity += THROWGRAVITY;
			objRock.pos = D3DXVECTOR3(objRock.posLerp.x, objRock.nowY, objRock.posLerp.z);

			// 궤도 파악 
			D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
			objRock.posList.push_back(VertexPC(objRock.pos, color));

			// 반복을 위해 
			if (objRock.nowY < 0) {
				throwStart = false;
				objRock.gravity = objRock.passedTime = 0.f;
				objRock.posList.clear();	// 궤도 비우기 
			}
		}
	}
}

void Bullymong::Assaulted(POINT mouse)
{
	//2D상의 점이
	//3D상의 구체와 충돌했을 때 
	//hp가 깎인다.

}


//-----------------------------------------------------------------------------
// Bullymong::ShowBSphere()
// 설명: 현재 지점에서 목표 지점까지의 방향을 구한다. 
//-----------------------------------------------------------------------------
void Bullymong::ShowBSphere()
{
	if (showBSphere == false)
		showBSphere = true;
	else
		showBSphere = false;
}
