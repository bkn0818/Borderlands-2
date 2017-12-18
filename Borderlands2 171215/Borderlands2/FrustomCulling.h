#pragma once

class FrustumCulling
{
public:
	FrustumCulling();
	~FrustumCulling();
	void Init(D3DXMATRIX* pMatViewProj);
	void Update();
	bool IsInFrustum(SphereInfo*);
	bool IsInSphere(D3DXVECTOR3* pv, float radius);
	bool IsIn(D3DXVECTOR3* pv);

	D3DXVECTOR3* GetPos() { return &vPos; };

private:
	std::vector<D3DXVECTOR3>	orgProjVertexList;
	std::vector<D3DXPLANE>		planeList;
	D3DXVECTOR3					vPos;
};

