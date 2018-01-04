#include "stdafx.h"
#include "SkinnedMeshManager.h"


SkinnedMeshManager::SkinnedMeshManager()
{
}


SkinnedMeshManager::~SkinnedMeshManager()
{
}

SkinnedMesh* SkinnedMeshManager::GetSkinnedMesh(char* szFolder, char* szFile)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFile);

	if (skinnedMeshMapList.find(sFullPath) == skinnedMeshMapList.end())
	{
		SkinnedMesh* pSkinnedMesh = new SkinnedMesh();
		pSkinnedMesh->Load(szFolder, szFile);
		skinnedMeshMapList[sFullPath] = pSkinnedMesh;
	}

	return skinnedMeshMapList[sFullPath];
}

// enemy 전용으로 skinnedMesh 중복 허용
SkinnedMesh* SkinnedMeshManager::GetEnemySkinnedMesh(char* szFolder, char* szFile)
{
	std::string sFullPath(szFolder);
	sFullPath += std::string(szFile);

	SkinnedMesh* pSkinnedMesh = new SkinnedMesh();
	pSkinnedMesh->Load(szFolder, szFile);

	enemySkinnedMeshList.push_back(pSkinnedMesh);

	return pSkinnedMesh;
}

void SkinnedMeshManager::Destroy()
{
	for each(auto it in skinnedMeshMapList)
	{
		it.second->Destroy();
	}
}
