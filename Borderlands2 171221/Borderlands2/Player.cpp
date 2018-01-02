#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}


Player::~Player()
{
}

void Player::Init()
{
	charCtrl = new CharacterCtrl;
	charCtrl->Init();

	//기본스탯설정
	_player.MaxHp = 10;
	_player.MaxSp = 10;
	_player.Hp = _player.MaxHp;
	_player.Sp = _player.MaxSp;

	_player.isDie = false;

	//플레이어 메쉬
	_player.skinnedMesh = new SkinnedMesh("./sjXFile/", "test_hand.X");
	_player.skinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	
	//상태초기화
	_player.stat = PLAYER_IDLE;
	_player.skinnedMesh->SetAnimationIndex(_player.stat);

	//무기 메쉬
	test = new SkinnedMesh("./sjXFile/", "testgun.X");

	///////////////
	ZeroMemory(&bulletPick, sizeof(D3DMATERIAL9));
	bulletPick.Diffuse = bulletPick.Ambient = bulletPick.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXCreateSphere(D3DDEVICE, 0.01f, 10.f, 10.f, &bulletTest, NULL);
	D3DXMatrixIdentity(&bulletMat);
}

void Player::Update(iMap* obj)
{
	//키컨트롤
	if(!_player.isDie)KeyCtrl();
	
	//무기 연결
	WeaponCtrl();

	//카메라 설정
	if (!_player.isDie)g_pCamera->SetEye(&_player.skinnedMesh->GetCamBonePos("Camera"));
	else g_pCamera->SetEye(&_player.skinnedMesh->GetCamBonePos("Head"));

	//캐릭터 컨트롤
	charCtrl->Update(obj);

	//캐릭터 포지션 이동
	if (!_player.isDie)_player.skinnedMesh->SetPosition(charCtrl->GetPosition());
	charCtrl->GetCameraLookAt(g_pCamera->GetLookAt(), g_pCamera->GetSide());

	Dead();
}

void Player::Render()
{
	if (_player.stat < PLAYER_JUMP_END) test->UpdateAndRender();
	_player.skinnedMesh->UpdateAndRender();

	D3DDEVICE->SetMaterial(&bulletPick);
	D3DXMatrixTranslation(&bulletMat, bulletPos.x, bulletPos.y, bulletPos.z);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &bulletMat);
	bulletTest->DrawSubset(0);

	LPD3DXFONT font = g_pFontManager->GetFont(g_pFontManager->QUEST);
	RECT rc = { 200, 850, 200, 200 };
	char str[1024];
	sprintf(str, "%d , %d", _player.Sp, _player.Hp);
	font->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 0, 255));
}

void Player::Recovery()
{
	if (_player.Sp < _player.MaxSp)
	{
		_player.Sp += 10;
		if (_player.Sp > _player.MaxSp)_player.Sp = _player.MaxSp;
	}
}

void Player::Hit(float dmg)
{
	if (_player.Sp > 0)
	{
		_player.Sp -= dmg;
		if (_player.Sp < 0) _player.Sp = 0;
	}
	else
	{
		_player.Hp -= dmg;
		if (_player.Hp <= 0)
		{
			_player.Hp = 0;
			_player.isDie = true;
		}
	}
}

void Player::Dead()
{
	if (_player.isDie)
	{
		_player.skinnedMesh = new SkinnedMesh("./sjXFile/", "playerTest.X");
		_player.skinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	}
	
}

void Player::WeaponCtrl()
{
	//본 파인드
	LPD3DXFRAME testGun = D3DXFrameFind((D3DXFRAME*)test->GetRoot(), "Root");
	LPD3DXFRAME testR = D3DXFrameFind((D3DXFRAME*)_player.skinnedMesh->GetRoot(), "R_Weapon_Bone");
	LPD3DXFRAME testL = D3DXFrameFind((D3DXFRAME*)_player.skinnedMesh->GetRoot(), "L_Weapon_Bone");
	
	//플레이어 무기 연결
	if (_player.stat < PLAYER_JUMP_END)testR->pFrameFirstChild = testGun;
}

void Player::KeyCtrl()
{
		///////////////////////////////////////////////전후좌우
		if (g_pKeyManager->IsStayKeyDown('W')
			|| g_pKeyManager->IsStayKeyDown('S')
			|| g_pKeyManager->IsStayKeyDown('A')
			|| g_pKeyManager->IsStayKeyDown('D'))
		{
			if (_player.stat == PLAYER_IDLE)
			{
				_player.stat = PLAYER_RUN_F;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}
			else if (_player.stat == PLAYER_SMG_IDLE)
			{
				_player.stat = PLAYER_SMG_RUN_F;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}
		}
		else
		{
			if (_player.stat > PLAYER_SMG_DRAW && _player.stat != PLAYER_IDLE)
			{
				_player.stat = PLAYER_IDLE;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}
			else if (_player.stat < PLAYER_JUMP_END&& _player.stat != PLAYER_SMG_IDLE)
			{
				_player.stat = PLAYER_SMG_IDLE;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}	
		}

		///////////////////////////////////////////////////////무기뽑기
		if (g_pKeyManager->IsOnceKeyDown('1') && _player.stat > PLAYER_SMG_DRAW)
		{
			_player.stat = PLAYER_SMG_DRAW;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);
		}
		if (_player.stat == PLAYER_SMG_DRAW && _player.skinnedMesh->GetAnimEnd(PLAYER_SMG_DRAW))
		{
			_player.stat = PLAYER_SMG_IDLE;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);
		}

		//////////////////////////////////////////////////////발사
		if (g_pKeyManager->IsStayKeyDown(VK_LBUTTON) && _player.stat < PLAYER_JUMP_END)
		{
			_player.stat = PLAYER_SMG_IDLE;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);

			g_pCamera->TestPt(true);
		}
		if (g_pKeyManager->IsOnceKeyUp(VK_LBUTTON))g_pCamera->TestPt(false);
}

void Player::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		/*bulletPos = D3DXVECTOR3( test->GetCamBoneMag05()._41
			, test->GetCamBoneMag05()._42
			, test->GetCamBoneMag05()._43);*/
		if (_player.stat < PLAYER_JUMP_END)
		{
			RayCtrl ray = ray.RayAtWorldSpace(LOWORD(lParam), HIWORD(lParam));
			bulletPos = ray.orgPosition + (ray.direction * 2);
		}
	}
	break;
	case WM_RBUTTONDOWN:
	{
		Hit(10.f);
	}
	break;
	}
}

D3DXVECTOR3 Player::GetPosition()
{
	return _player.skinnedMesh->GetPosition();
}

SphereInfo * Player::GetSphere()
{
	return _player.skinnedMesh->GetBoundingSphere();
}
