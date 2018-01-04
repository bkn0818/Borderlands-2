#pragma once
#include "SkinnedMesh.h"

#define g_pSkinnedMeshManager SkinnedMeshManager::GetInstance()

class SkinnedMeshManager
{
	SINGLETON ( SkinnedMeshManager );

public:
	SkinnedMesh*	GetSkinnedMesh ( char* szFolder, char* szFile );
	SkinnedMesh*	GetEnemySkinnedMesh(char* szFolder, char* szFile); // enemy 전용
	void			Destroy ();


private:
	std::map<std::string, SkinnedMesh*> skinnedMeshMapList;
	std::vector<SkinnedMesh*>			enemySkinnedMeshList;			// enemy 전용
};

// 같은 경로의 x파일 호출 시 같은 skinnedMesh를 공유하는 문제 확인.
// enemy 전용으로 skinnedMesh 중복을 허용하여 리턴하는 함수, vector 별도 선언함.