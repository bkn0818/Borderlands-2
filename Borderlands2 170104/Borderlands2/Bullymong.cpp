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
	curAnimSet = BMONG_WALK;

	sm = g_pSkinnedMeshManager->GetSkinnedMesh("./XFile/", "bullymong.X");
	sm->SetMoving(false);
	sm->SetPosition(pos);
	sm->SetAnimationIndex(curAnimSet);
	sm->RotateY(-30);						// �� ���Բ�

	hp = HPMAX_BMONG;

	// bounding Sphere============================================
	SetBoundingSphere(stBSphere[SPHERE_HEAD], 2.0f, 255, 0, 0);
	SetBoundingSphere(stBSphere[SPHERE_ASSAULT], 2.0f, 0, 0, 255);
	SetBoundingSphere(stBSphere[SPHERE_RECOG], 20.0f, 255, 0, 0);
	SetBoundingSphere(stBSphere[SPHERE_ATTACK], 10.f, 255, 0, 0);


	// ������ ��ü================================================
	D3DXCreateSphere(D3DDEVICE, 0.2f, 20, 20, &objRock.mesh, NULL);
	objRock.gravity = 0.f;
	objRock.passedTime = 0.f;
	objRock.throwTime = 5.f;

	D3DXCOLOR color = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
	ZeroMemory(&objRock.material, sizeof(D3DMATERIAL9));
	objRock.material.Ambient = objRock.material.Diffuse = objRock.material.Specular = color;


	isRecog = false;
	showBSphere = false;

	passedActionTime = 0.0f;
	moveInterval = 0.1f;

	isThrow = false;
	throwStart = false;
	count = 0;

	animFrame[BMONG_RUN_FRONT] = 21;
	animFrame[BMONG_WALK] = 31;
	animFrame[BMONG_IDLE] = 61;
	animFrame[BMONG_HIT] = 53;
	animFrame[BMONG_DEATH] = 1;
	animFrame[BMONG_THROWROCK] = 65;
	animFrame[BMONG_PUNCH_LEFT] = 82;
	animFrame[BMONG_PUNCH_RIGHT] = 49;

	passedTime = 0.f;

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
	Move(posPlayer);
	ThrowObj(posPlayer);

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
	sprintf(str, "dis:%.3f, E(%.3f, %.3f, %3.f) P(%.3f, %.3f, %.3f) curAnim: %d", distance, pos.x, pos.y, pos.z, px, py, pz, curAnimSet);
	font->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 0));

	LPD3DXFONT font2 = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc2 = { 400, 60, 600, 100 };
	char str2[1024];
	sprintf(str2, "R(%.3f, %.3f, %.3f) R_TAR(%.3f, %.3f, %.3f) Angle: %.3f", objRock.pos.x, objRock.pos.y, objRock.pos.z, objRock.posTarget.x, objRock.posTarget.y, objRock.posTarget.z, objRock.angle);
	font2->DrawTextA(nullptr, str2, strlen(str2), &rc2, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(0, 255, 255));

	LPD3DXFONT font3 = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc3 = { 400, 130, 600, 150 };
	char str3[1024];
	sprintf(str3, "passedTime: %.3f", passedTime);
	font2->DrawTextA(nullptr, str3, strlen(str3), &rc3, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 0));


	sm->UpdateAndRender();

	// bounding Sphere========================================
	D3DXMATRIXA16 matWorld;

	D3DXMatrixTranslation(&matWorld,
		pos.x,
		pos.y,
		pos.z);

	D3DDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DDEVICE->SetMaterial(&stBSphere[SPHERE_RECOG].material);

	stBSphere[SPHERE_RECOG].sphereMesh->DrawSubset(0);

	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // ������� 

	//	for each(auto p in range)
	//	{
	//		p.sphereMesh->DrawSubset(0);
	//	}

	// =======================================================

	// throwObj===============================================
	D3DXMATRIXA16 rockWorld;

	D3DXMatrixTranslation(&rockWorld,
		objRock.pos.x,
		objRock.pos.y,
		objRock.pos.z);

	D3DDEVICE->SetTransform(D3DTS_WORLD, &rockWorld);
	D3DDEVICE->SetMaterial(&objRock.material);
	objRock.mesh->DrawSubset(0);

	// rock Ȯ�ο�====================================
	D3DXMATRIXA16 testMT;
	D3DXMatrixIdentity(&testMT);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &testMT);
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
}


//-----------------------------------------------------------------------------
// Bullymong::Release()
// ����: 
//-----------------------------------------------------------------------------
void Bullymong::Release()
{
}


//-----------------------------------------------------------------------------
// Bullymong::Move(D3DXVECTOR3 posPlayer)
// ����: �̵� 
//-----------------------------------------------------------------------------
void Bullymong::Move(D3DXVECTOR3 posPlayer)
{
	srand(time(NULL));
	int rNum = rand() % 2;
	bool isMoveEnd = false; // �� �������°� 

							// 1. �ν�O: THROWROCK Ȥ�� RUN Ȥ�� PUNCH
	if (isRecog) {
		float closeAtkRange = stBSphere[SPHERE_ATTACK].sphereInfo.fRadius;
		// 2. hp�� 40% �̻� && �÷��̾���� ������ ���� ���� ���� �ʰ��� ��: ������
		if (hp > HPMAX_BMONG * 0.4 && distance > closeAtkRange) {
			if (curAnimSet != BMONG_THROWROCK) {
				curAnimSet = BMONG_THROWROCK;
				sm->SetAnimationIndex(curAnimSet);
			}
			else if (curAnimSet == BMONG_THROWROCK) {
				if (!isThrow) {
					isThrow = true; // �� ������ ���� 
					throwStart = true;
					objRock.pos = D3DXVECTOR3(pos.x - 1.f, pos.y + 5, pos.z + 3); // �ȶ� ���...
					objRock.posTarget = posPlayer; // �÷��̾� ��ġ�� �°Բ� ��ǥ�� ��´� 
					objRock.nowY = objRock.pos.y;
					objRock.posLerp = objRock.pos;

					// ���� �������� �÷��̾� ��ǥ �� ����==================
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

					D3DXVECTOR3 vSub;
					D3DXVec3Subtract(&vSub, &objRock.posTarget, &objRock.pos);

					a = D3DXVec3Length(&vSub);
					b = objRock.pos.x - objRock.posTarget.x;
					cos = b / a;
					objRock.angle = acosf(cos) * (180 / PI);

					// �� �̵� �˵��� ���� ����==============================
					D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
					objRock.posList.push_back(VertexPC(objRock.pos, color));
				}
			}
		}
		// 2. hp�� 40% �̸� OR �÷��̾���� ������ ���� ���� ���� ������ ��: ��ġ
		else if (hp > HPMAX_BMONG * 0.3 || distance <= closeAtkRange) {
		}
		// 3. �÷��̾���� ������ �ν� ����*2�̻��� �� �ν� ���� 
		else if (distance >= stBSphere[SPHERE_RECOG].sphereInfo.fRadius * 2)
		{
			isRecog = false;
		}
	}
	// 1. �ν�X: WALK Ȥ�� IDLE
	else {
		// 2. WALK�� IDLE�� �ƴ϶��: �� �� �ϳ���  
		if (curAnimSet != BMONG_WALK && curAnimSet != BMONG_IDLE)
		{
			if (!rNum)	curAnimSet = BMONG_WALK;
			else		curAnimSet = BMONG_IDLE;
			sm->SetAnimationIndex(curAnimSet);
		}
	}
	/*
	// 2. WALK�� IDLE�� �ƴ�: �� �� �ϳ���
	if (curAnim != BMONG_WALK && curAnim != BMONG_IDLE)
	{
	isMoveEnd = true;
	}
	else
	{
	passedActionTime += g_pTimeManager->GetDeltaTime();

	// 2. ���� WALK: ��ǥ�� ������ ������ �̵�
	if (curAnim == BMONG_WALK)
	{
	// 3. �����̱� ����O: ��������
	if (isMoveStart)
	{
	// 4. ���� ������ ��ǥ ������ �Ÿ��� ����: �׸� �̵�����
	D3DXVECTOR3 dis = targetPos - pos;

	if (D3DXVec3Length(&dis) <= EPSILON)
	{
	isMoveEnd = true;	//�����ߴ�
	}
	// 4. ���� ������ ��ǥ ������ �Ÿ��� �ٸ�: �̵�����
	else
	{
	// 5. �̵� �Ҹ��� ��������
	if (passedActionTime < moveInterval)
	{
	float t = passedActionTime / moveInterval;	// ��
	D3DXVECTOR3 p;
	D3DXVec3Lerp(&p, &pos, &targetPos, t); // ����position���� targetPosition���� ��������
	pos = p;
	}
	}
	}
	// 3. �����̱� ����X: �� ���� �����̾� + ��ǥ���� ����
	else
	{
	isMoveStart = true;
	int a = rand() % 15;
	int b = rand() % 10;
	targetPos = D3DXVECTOR3(pos.x + a - b, pos.y, pos.z + a - b);	// ���� ��ġ
	}
	}
	// 2. ���� IDLE: �ð� ������ �ʱ�ȭ
	else if (curAnim == BMONG_IDLE)
	{
	if (passedActionTime > moveInterval)
	isMoveEnd = true;
	}
	}
	// �������� �������� �ʱ�ȭ
	if (isMoveEnd)
	{
	isMoveStart = false; // ��ǥ������ ��������
	passedActionTime = 0.f;
	rNum = rand() % 2;

	if (!rNum)		curAnim = BMONG_WALK;
	else if (rNum)	curAnim = BMONG_IDLE;
	sm->SetAnimationIndex(curAnim);
	}
	}
	*/
}


//-----------------------------------------------------------------------------
// Bullymong::ThrowObj(D3DXVECTOR3 posPlayer)
// ����: player�� ��ǥ�� ��ü�� ������������ �����ؼ� objRock�� ��� ������ 
//-----------------------------------------------------------------------------
void Bullymong::ThrowObj(D3DXVECTOR3 posPlayer)
{
	if (curAnimSet == BMONG_THROWROCK) {
		if (isThrow) {
			// ������ Ÿ�̹� ���� �ʿ�.
			if (throwStart) {
				objRock.passedTime += g_pTimeManager->GetDeltaTime();

				if (objRock.passedTime < objRock.throwTime) {
					float t = objRock.passedTime / objRock.throwTime;

					D3DXVECTOR3 p;
					D3DXVec3Lerp(&p, &objRock.posLerp, &objRock.posTarget, t); // ����position���� targetPosition���� �������� 

					objRock.posLerp = p;
					objRock.nowY += 0.1f - objRock.gravity;	// �߷��� �������� 
					objRock.gravity += 0.01f;
					objRock.pos = D3DXVECTOR3(objRock.posLerp.x, objRock.nowY, objRock.posLerp.z);

					// �˵� �ľ� 
					D3DCOLOR color = D3DCOLOR_XRGB(255, 0, 0);
					objRock.posList.push_back(VertexPC(objRock.pos, color));

					// �ݺ��� ���� 
					if (objRock.nowY < 0) {
						isThrow = false;	// ������ ��
						throwStart = false;
						objRock.gravity = objRock.passedTime = 0.f;
						objRock.posList.clear();	// �˵� ���� 
					}
				}
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


void Bullymong::Attack()
{

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
