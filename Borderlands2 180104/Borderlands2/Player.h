#pragma once
class iMap;
class SkinnedMesh;
class CharacterCtrl;

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
	//����ó���� ����
	PlayerStat		stat;
	//�޽�
	SkinnedMesh*	skinnedMesh;
	//�÷��̾� ü��
	int				Hp;
	int				MaxHp;
	//�÷��̾� �ǵ�
	int				Sp;
	int				MaxSp;
	
	bool			isDie;
};

class Player
{
private:
	tagAssassin		_player;
	CharacterCtrl*	charCtrl;
	SkinnedMesh*	test;

	LPD3DXMESH		bulletTest;
	D3DXMATRIX		bulletMat;
	D3DMATERIAL9	bulletPick;
	D3DXVECTOR3		bulletPos;

public:
	void Init();
	void Update(iMap* obj);
	void Render();

	void Recovery();
	void Hit(float dmg);
	void Dead();
	void WeaponCtrl();
	void KeyCtrl();
	void MainProc(HWND, UINT, WPARAM, LPARAM);

	// ���� �߰�============ 
	D3DXVECTOR3 GetPosition();
	SphereInfo* GetSphere();
	// ===================== 

	Player();
	~Player();
};