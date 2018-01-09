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
// ����: �ʱ�ȭ 
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
	sm->RotateY(curAngle);					// �� ���Բ�

//	hp = HPMAX_BMONG;
	hp = 230;								// ���� ���� �׽�Ʈ��

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


	// ������ ��ü================================================
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
// ����: imap, �÷��̾��� ��ǥ�� boundingSphere�� �޾� ������Ʈ 
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
// ����: draw
//-----------------------------------------------------------------------------
void Bullymong::Render()
{
	// �Ÿ� �׽�Ʈ 
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

	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // ������� 

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
	// rock Ȯ�ο�============================================
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
// ����: 
//-----------------------------------------------------------------------------
void Bullymong::Release()
{
}


//-----------------------------------------------------------------------------
// Bullymong::SetState(D3DXVECTOR3 posPlayer)
// ����: ���� ���� 
//-----------------------------------------------------------------------------
void Bullymong::SetState(D3DXVECTOR3* posPlayer)
{
	// �÷��̾�� enemy�� ���� ===============================
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

	// ���� ���� ���� 
	float closeAtkRange = stBSphere[SP_ATTACK].sphereInfo.fRadius;

	// 1. �ν�O: THROWROCK / RUN / PUNCH
	if (isRecog) {
		// 2. �÷��̾���� ������ ���� ���� ���� ��: THROWROCK / RUN
		if (distance > closeAtkRange) {
			// 3. HP 40% �̻�: THROWROCK
			if (hp >= HPMAX_BMONG * 0.4) {
				if (!isAttack) {
					isAttack = true;
					curAnim = BMONG_THROWROCK;
				}
				else {
					// 4�ʸ��� THROW / RUN
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
						objRock.posList.clear();	// �˵� ���� 
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
			// 3. HP 40% �̸�: RUN(���������Ϸ�)
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
		// 2. ���� ���� ���� ��: PUNCH
		else if (distance <= closeAtkRange) {
			isAttack = true;
			if (posPlayer->z > pos.z && posPlayer->x <= pos.x) { // 3. �÷��̾� ��ġ�� ���� ��/�� ��ġ  
				curAnim = BMONG_PUNCH_RIGHT;
			}
			else if(posPlayer->z <= pos.z && posPlayer->x > pos.x){
				curAnim = BMONG_PUNCH_LEFT;
			}
			sm->SetAnimationIndex(curAnim);
		}
		// 2. �ν� ���� * 2�̻�: �ν� ���� 
		else if (distance >= stBSphere[SP_RECOG].sphereInfo.fRadius * 2) {
			isRecog = false;
		}
	}
	// 1. �ν�X: WALK / IDLE
	else {
	}

	termAnimate -= g_pTimeManager->GetDeltaTime();
}


//-----------------------------------------------------------------------------
// Bullymong::Move(D3DXVECTOR3 posPlayer)
// ����: �̵� 
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
		// throw ���� �� �¿�� �����δ�.
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
// ����: ����
//-----------------------------------------------------------------------------
void Bullymong::Attack()
{

}


//-----------------------------------------------------------------------------
// Bullymong::ThrowObj(D3DXVECTOR3 posPlayer)
// ����: player�� ��ǥ�� ��ü�� ������������ �����ؼ� objRock�� ��� ������ 
//-----------------------------------------------------------------------------
void Bullymong::ThrowObj(D3DXVECTOR3* posPlayer)
{
	// ��ǥ ����
	if (!throwStart) {
		throwStart = true;
		objRock.pos = D3DXVECTOR3(pos.x - 1.f, pos.y + 5, pos.z + 3);
		objRock.posTarget = *posPlayer; // �÷��̾ ��Բ� 
		objRock.nowY = objRock.pos.y;
		objRock.posLerp = objRock.pos;

		// ���� �������� �÷��̾� ��ǥ �� ����==================
		float c, a, b;

		D3DXVECTOR3 vSub;
		D3DXVec3Subtract(&vSub, &objRock.posTarget, &objRock.pos);

		a = D3DXVec3Length(&vSub);
		b = objRock.pos.x - objRock.posTarget.x;
		c = b / a;
		objRock.angle = acosf(c) * (180 / PI);

		// �� �̵� �˵�=========================================
		D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
		objRock.posList.push_back(VertexPC(objRock.pos, color));
	}
	// ������ 
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

			// �˵� �ľ� 
			D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
			objRock.posList.push_back(VertexPC(objRock.pos, color));

			// �ݺ��� ���� 
			if (objRock.nowY < 0) {
				throwStart = false;
				objRock.gravity = objRock.passedTime = 0.f;
				objRock.posList.clear();	// �˵� ���� 
			}
		}
	}
}

void Bullymong::Assaulted(POINT mouse)
{
	//2D���� ����
	//3D���� ��ü�� �浹���� �� 
	//hp�� ���δ�.

}


//-----------------------------------------------------------------------------
// Bullymong::ShowBSphere()
// ����: ���� �������� ��ǥ ���������� ������ ���Ѵ�. 
//-----------------------------------------------------------------------------
void Bullymong::ShowBSphere()
{
	if (showBSphere == false)
		showBSphere = true;
	else
		showBSphere = false;
}
