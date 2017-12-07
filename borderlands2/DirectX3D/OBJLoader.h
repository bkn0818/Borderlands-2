#pragma once

class MaterialTexture;
class Group;

class OBJLoader : public Object
{
public:
	OBJLoader();
	~OBJLoader();

	void SetPath(char* path);
	void Load(IN char* szFullPath,
			  IN D3DXMATRIX* pmat,
			  OUT std::vector<Group*>& vecGroup);
	LPD3DXMESH LoadMesh(IN char* filename, IN D3DXMATRIX* pMat, OUT std::vector<MaterialTexture*> &mtList);

protected:
	void LoadMtlLib(char* szFullPath);

protected:
	std::map<std::string, MaterialTexture*>  mapMaterialTexture;
	std::string path;
};