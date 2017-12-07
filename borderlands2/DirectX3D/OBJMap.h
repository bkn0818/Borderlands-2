#pragma once
class OBJMap : public iMap
{
protected:
	std::vector<D3DXVECTOR3> m_vecVertex;

public:
	OBJMap();
	virtual ~OBJMap();

	virtual void Load(char* szFullPath, D3DXMATRIX pmat);
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z);
};

