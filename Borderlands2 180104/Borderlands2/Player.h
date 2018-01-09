#pragma once

class iMap;
class SkinnedMesh;
class CharacterCtrl;
class UIManager;
class UIObject;
class UIImageView;

enum PlayerStat
{
	PLAYER_ADD_FURE_RECOIL = 0,
	PLAYER_SMG_BMADE_RELOAD,
	PLAYER_SMG_GRENADE,

	PLAYER_SMG_JUMP_END,
	PLAYER_SMG_JUMP_IDLE,
	PLAYER_SMG_JUMP_START,
	
	PLAYER_SMG_SPRINT,
	PLAYER_SMG_RUN_R,
	PLAYER_SMG_RUN_L,
	PLAYER_SMG_RUN_F,

	PLAYER_SMG_IDLE,
	PLAYER_SMG_HOLSTER,
	PLAYER_SMG_DRAW,

	PLAYER_JUMP_END,
	PLAYER_JUMP_IDLE,
	PLAYER_JUMP_START,

	PLAYER_INCAP_F,
	PLAYER_RUN_L,
	PLAYER_RUN_R,
	PLAYER_RUN_F,

	PLAYER_SPRINT,
	PLAYER_WALK,
	PLAYER_IDLE,
	
	PLAYER_END
};

struct tagAssassin
{
	//상태처리용 스탯
	PlayerStat		stat;
	//메쉬
	SkinnedMesh*	skinnedMesh;
	//플레이어 체력
	int				Hp;
	int				MaxHp;
	
	int				armo;
	int				bullet;
	int				maxBullet;

	bool			isDie;
};

class Player
{
private:
	tagAssassin		_player;
	CharacterCtrl*	charCtrl;
	SkinnedMesh*	test;
	UIManager*		ui;

	LPD3DXMESH		bulletTest;
	D3DXMATRIX		bulletMat;
	D3DMATERIAL9	bulletPick;
	D3DXVECTOR3		bulletPos;
	WPARAM			WParam; 
	LPARAM			LParam;

	LPD3DXSPRITE	aimSprite;
	UIObject*		AimRoot;
	UIImageView*	AimIma;

	int				n = 8;

public:
	void Init();
	void Update(iMap* obj);
	void Render();

	void Hit(float dmg);
	void Dead();
	void WeaponCtrl();
	void KeyCtrl();
	void MainProc(HWND, UINT, WPARAM, LPARAM);
	void AimCtrl();

	// 지영 추가============ 
	D3DXVECTOR3 GetPosition();
	SphereInfo* GetSphere();
	// ===================== 

	Player();
	~Player();
};