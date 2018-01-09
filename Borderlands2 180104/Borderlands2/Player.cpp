#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}


Player::~Player()
{
	//AimRoot->Destroy();
}

void Player::Init()
{
	charCtrl = new CharacterCtrl;
	charCtrl->Init();

	//기본스탯설정
	_player.MaxHp = 100;
	_player.Hp = _player.MaxHp;

	_player.armo = 100;
	_player.maxBullet = 30;
	_player.bullet = _player.maxBullet;

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

	AimCtrl();

	ui = new UIManager;
	ui->Init(_player.MaxHp);
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

	ui->Update();

	
}

void Player::Render()
{
	if (_player.stat < PLAYER_JUMP_END) test->UpdateAndRender();
	_player.skinnedMesh->UpdateAndRender();

	D3DDEVICE->SetMaterial(&bulletPick);
	D3DXMatrixTranslation(&bulletMat, bulletPos.x, bulletPos.y, bulletPos.z);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &bulletMat);
	bulletTest->DrawSubset(0);

	/*LPD3DXFONT font = g_pFontManager->GetFont(g_pFontManager->UI);
	RECT rc = { 200, 850, 200, 200 };
	char str[1024];
	sprintf(str, "%d , %d", _player.Sp, _player.Hp);
	font->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_TOP | DT_NOCLIP, D3DCOLOR_XRGB(255, 0, 255));*/
	ui->Render();
	AimRoot->Render(aimSprite, nullptr);
}

void Player::Hit(float dmg)
{
	_player.Hp -= dmg;
	ui->OnAttacked(dmg);
	if (_player.Hp <= 0)
	{
		_player.Hp = 0;
		_player.isDie = true;
	}
}

void Player::Dead()
{
	if (_player.isDie)
	{
		D3DXVECTOR3 pos = _player.skinnedMesh->GetPosition();
		_player.skinnedMesh = new SkinnedMesh("./sjXFile/", "playerTest.X");
		_player.skinnedMesh->SetPosition(pos);
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
			if (_player.stat == PLAYER_IDLE )
			{
				_player.stat = PLAYER_RUN_F;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}
			else if (_player.stat == PLAYER_SMG_IDLE
				&& _player.stat != PLAYER_SMG_BMADE_RELOAD)
			{
				_player.stat = PLAYER_SMG_RUN_F;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}
		}
		else
		{
			if (_player.stat > PLAYER_SMG_DRAW 
				&& _player.stat != PLAYER_IDLE)
			{
				_player.stat = PLAYER_IDLE;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}
			else if (_player.stat < PLAYER_JUMP_END
				&& _player.stat != PLAYER_SMG_IDLE
				&& _player.stat != PLAYER_SMG_BMADE_RELOAD)
			{
				_player.stat = PLAYER_SMG_IDLE;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}	
		}

		///////////////////////////////////////////////////////무기뽑기
		if (g_pKeyManager->IsOnceKeyDown('1') 
			&& _player.stat > PLAYER_SMG_DRAW)
		{
			_player.stat = PLAYER_SMG_DRAW;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);
		}

		if (_player.stat == PLAYER_SMG_DRAW 
			&& _player.skinnedMesh->GetAnimEnd(PLAYER_SMG_DRAW))
		{
			_player.stat = PLAYER_SMG_IDLE;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);
		}

		//////////////////////////////////////////////////////발사
		if (g_pKeyManager->IsStayKeyDown(VK_LBUTTON) 
			&& _player.stat < PLAYER_JUMP_END 
			&& _player.bullet > 0
			&& _player.stat != PLAYER_SMG_BMADE_RELOAD)
		{
			_player.stat = PLAYER_SMG_IDLE;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);

			
			if (n % 8 == 0)
			{
				_player.bullet--;
				ui->OnClick(1);
				RayCtrl ray = ray.RayAtWorldSpace(LOWORD(LParam), HIWORD(LParam));
				bulletPos = ray.orgPosition + (ray.direction * 2);
			}
			++n;
			g_pCamera->TestPt(true);
		}
		if (g_pKeyManager->IsOnceKeyUp(VK_LBUTTON)) 
		{
			n = 8;
			g_pCamera->TestPt(false);
		}

		if (g_pKeyManager->IsOnceKeyDown('R'))
		{
			if (_player.bullet != _player.maxBullet)
			{
				_player.stat = PLAYER_SMG_BMADE_RELOAD;
				_player.skinnedMesh->SetAnimationIndex(_player.stat);
			}
		}
		if (_player.stat == PLAYER_SMG_BMADE_RELOAD 
			&& _player.skinnedMesh->GetAnimEnd(PLAYER_SMG_BMADE_RELOAD))
		{
			_player.bullet += (_player.maxBullet - _player.bullet);
			_player.stat = PLAYER_SMG_IDLE;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);
		}
}

void Player::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WParam = wParam;
	LParam = lParam;
}

void Player::AimCtrl()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	D3DXIMAGE_INFO info;
	AimRoot = new UIObject;
	AimIma = new UIImageView;
	D3DXCreateSprite(D3DDEVICE, &aimSprite);
	LPDIRECT3DTEXTURE9 texture = g_pTextureManager->GetTexture("sjXFile/cross 03.png", &info);
	AimIma->SetTexture(texture);
	AimIma->SetSize(D3DXVECTOR2(info.Width, info.Height));
	float x = ((rc.right - rc.left) / 2) - (info.Width / 2);
	float y = ((rc.bottom - rc.top) / 2) - (info.Height / 2);
	AimIma->SetLocalPos(D3DXVECTOR3(x, y, 0));
	AimRoot = AimIma;
}

D3DXVECTOR3 Player::GetPosition()
{
	return _player.skinnedMesh->GetPosition();
}

SphereInfo * Player::GetSphere()
{
	return _player.skinnedMesh->GetBoundingSphere();
}
