#pragma once
class FrustumCulling
{
public:
	FrustumCulling ();
	~FrustumCulling ();
	void Update ();
	bool IsInFrustum ( SphereInfo* );

private:
	std::vector<D3DXVECTOR3>	orgProjVertexList;
	std::vector<D3DXPLANE>		planeList;

};

