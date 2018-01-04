#pragma once
#include "SkinnedMesh.h"

#define g_pSkinnedMeshManager SkinnedMeshManager::GetInstance()

class SkinnedMeshManager
{
	SINGLETON ( SkinnedMeshManager );

public:
	SkinnedMesh*	GetSkinnedMesh ( char* szFolder, char* szFile );
	SkinnedMesh*	GetEnemySkinnedMesh(char* szFolder, char* szFile); // enemy ����
	void			Destroy ();


private:
	std::map<std::string, SkinnedMesh*> skinnedMeshMapList;
	std::vector<SkinnedMesh*>			enemySkinnedMeshList;			// enemy ����
};

// ���� ����� x���� ȣ�� �� ���� skinnedMesh�� �����ϴ� ���� Ȯ��.
// enemy �������� skinnedMesh �ߺ��� ����Ͽ� �����ϴ� �Լ�, vector ���� ������.