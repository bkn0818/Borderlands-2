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
	//상태초기화
	_player.stat = PLAYER_IDLE;

	charCtrl = new CharacterCtrl;
	charCtrl->Init();

	//기본스탯설정
	_player.MaxHp = 100;
	_player.MaxSp = 100;
	_player.Hp = _player.MaxHp;
	_player.Sp = _player.MaxSp;

	//메쉬추가
	_player.skinnedMesh = new SkinnedMesh("./sjXFile/", "test_hand.X");
	_player.skinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));

	_player.skinnedMesh->SetMoving(true);

	test = new SkinnedMesh("./sjXFile/", "testgun.X");
	//test->SetAnimationIndex(0);
	//test->SetPosition(D3DXVECTOR3(0, 0, 0));
}

void Player::Update(iMap* obj)
{
	_player.skinnedMesh->SetAnimationIndex(_player.stat /*true*/);
	WeaponCtrl();

	g_pCamera->SetEye(&_player.skinnedMesh->GetCamBonePos());
	charCtrl->Update(obj);
	_player.skinnedMesh->SetPosition(*charCtrl->GetPosition());
	charCtrl->GetCameraLookAt(g_pCamera->GetLookAt(), g_pCamera->GetSide());
}

void Player::Render()
{
	//if (_player.stat < PLAYER_JUMP_END) test->UpdateAndRender();
	_player.skinnedMesh->UpdateAndRender();
}

void Player::WeaponCtrl()
{
	LPD3DXFRAME testGun = D3DXFrameFind((D3DXFRAME*)test->GetRoot(), "Root");
	LPD3DXFRAME testR = D3DXFrameFind((D3DXFRAME*)_player.skinnedMesh->GetRoot(), "R_Weapon_Bone");
	LPD3DXFRAME testL = D3DXFrameFind((D3DXFRAME*)_player.skinnedMesh->GetRoot(), "L_Weapon_Bone");

	if (_player.stat < PLAYER_JUMP_END)testR->pFrameFirstChild = testGun;

	if (GetKeyState('W') & 0x8000
		|| GetKeyState('S') & 0x8000
		|| GetKeyState('A') & 0x8000
		|| GetKeyState('D') & 0x8000)
	{
		if (_player.stat == PLAYER_IDLE)_player.stat = PLAYER_RUN_F;
		else if (_player.stat == PLAYER_SMG_IDLE)_player.stat = PLAYER_SMG_RUN_F;
	}
	else
	{
		if (_player.stat > PLAYER_SMG_DRAW)_player.stat = PLAYER_IDLE;
		else if (_player.stat < PLAYER_JUMP_END)_player.stat = PLAYER_SMG_IDLE;
	}

	if (GetKeyState('1') & 0x8000)
	{
		if (_player.stat == PLAYER_IDLE || PLAYER_RUN_F)_player.stat = PLAYER_SMG_DRAW;
	}
}

void Player::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}

D3DXVECTOR3 Player::GetPosition()
{
	return _player.skinnedMesh->GetPosition();
}

SphereInfo* Player::GetSphere()
{
	return _player.skinnedMesh->GetBoundingSphere();
}