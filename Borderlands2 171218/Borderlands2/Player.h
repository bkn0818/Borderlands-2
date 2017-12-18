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

struct Assassin
{
	//상태처리용 스탯
	PlayerStat		stat;
	//메쉬
	SkinnedMesh*	skinnedMesh;
	//플레이어 체력
	int				Hp;
	int				MaxHp;
	//플레이어 실드
	int				Sp;
	int				MaxSp;
};

class Player
{
private:
	Assassin _player;
	//LPD3DXFRAME testCam;
	CharacterCtrl*	charCtrl;
	SkinnedMesh* test;
public:
	void Init();
	void Update(iMap* obj);
	void Render();

	void WeaponCtrl();
	void MainProc(HWND, UINT, WPARAM, LPARAM);

	Player();
	~Player();
};

