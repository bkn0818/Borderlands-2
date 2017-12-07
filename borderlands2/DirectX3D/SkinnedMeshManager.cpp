#include "stdafx.h"
#include "SkinnedMeshManager.h"


SkinnedMeshManager::SkinnedMeshManager()
{
}


SkinnedMeshManager::~SkinnedMeshManager()
{
}

SkinnedMesh * SkinnedMeshManager::GetSkinnedMesh(char * szFolder, char * szFile)
{
	std::string fullPath = szFolder + std::string("\\") + szFile;

	if (skinnedMeshMapList.find(fullPath) == skinnedMeshMapList.end()) 
	{
		SkinnedMesh* skinnedMesh = new SkinnedMesh;
		skinnedMesh->Load(szFolder, szFile);

		skinnedMeshMapList[fullPath] = skinnedMesh;
	}

	return skinnedMeshMapList[fullPath];
}

void SkinnedMeshManager::Destroy()
{
	for each(auto p in skinnedMeshMapList)
		p.second->Destroy();
}
