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

	//�⺻���ȼ���
	_player.MaxHp = 100;
	_player.Hp = _player.MaxHp;

	_player.armo = 100;
	_player.maxBullet = 30;
	_player.bullet = _player.maxBullet;

	_player.isDie = false;

	//�÷��̾� �޽�
	_player.skinnedMesh = new SkinnedMesh("./sjXFile/", "test_hand.X");
	_player.skinnedMesh->SetPosition(D3DXVECTOR3(0, 0, 0));
	
	//�����ʱ�ȭ
	_player.stat = PLAYER_IDLE;
	_player.skinnedMesh->SetAnimationIndex(_player.stat);

	//���� �޽�
	test = new SkinnedMesh("./sjXFile/", "testgun.X");

	///////////////
	ZeroMemory(&bulletPick, sizeof(D3DMATERIAL9));
	bulletPick.Diffuse = bulletPick.Ambient = bulletPick.Specular = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXCreateSphere(D3DDEVICE, 0.01f, 10.f, 10.f, &bulletTest, NULL);
	D3DXMatrixIdentity(&bulletMat);

	ui = new UIManager;
	ui->Init(_player.MaxHp);
}

void Player::Update(iMap* obj)
{
	//Ű��Ʈ��
	if(!_player.isDie)KeyCtrl();
	
	//���� ����
	WeaponCtrl();

	//ī�޶� ����
	if (!_player.isDie)g_pCamera->SetEye(&_player.skinnedMesh->GetCamBonePos("Camera"));
	else g_pCamera->SetEye(&_player.skinnedMesh->GetCamBonePos("Head"));

	//ĳ���� ��Ʈ��
	charCtrl->Update(obj);

	//ĳ���� ������ �̵�
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
	//�� ���ε�
	LPD3DXFRAME testGun = D3DXFrameFind((D3DXFRAME*)test->GetRoot(), "Root");
	LPD3DXFRAME testR = D3DXFrameFind((D3DXFRAME*)_player.skinnedMesh->GetRoot(), "R_Weapon_Bone");
	LPD3DXFRAME testL = D3DXFrameFind((D3DXFRAME*)_player.skinnedMesh->GetRoot(), "L_Weapon_Bone");
	
	//�÷��̾� ���� ����
	if (_player.stat < PLAYER_JUMP_END)testR->pFrameFirstChild = testGun;
}

void Player::KeyCtrl()
{
		///////////////////////////////////////////////�����¿�
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

		///////////////////////////////////////////////////////����̱�
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

		//////////////////////////////////////////////////////�߻�
		if (g_pKeyManager->IsStayKeyDown(VK_LBUTTON) && _player.stat < PLAYER_JUMP_END && _player.bullet > 0)
		{
			_player.stat = PLAYER_SMG_IDLE;
			_player.skinnedMesh->SetAnimationIndex(_player.stat);

			n++;
			if (n % 8 == 0)
			{
				_player.bullet--;
				ui->OnClick(1);
				RayCtrl ray = ray.RayAtWorldSpace(LOWORD(LParam), HIWORD(LParam));
				bulletPos = ray.orgPosition + (ray.direction * 2);
			}

			g_pCamera->TestPt(true);
		}
		if (g_pKeyManager->IsOnceKeyUp(VK_LBUTTON)) 
		{
			n = 0;
			g_pCamera->TestPt(false);
		}
}

void Player::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WParam = wParam;
	LParam = lParam;
}

D3DXVECTOR3 Player::GetPosition()
{
	return _player.skinnedMesh->GetPosition();
}

SphereInfo * Player::GetSphere()
{
	return _player.skinnedMesh->GetBoundingSphere();
}
