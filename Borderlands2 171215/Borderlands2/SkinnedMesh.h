#pragma once

struct Bone;

class SkinnedMesh
{
	friend class SkinnedMeshManager;


public:
	SkinnedMesh ( char* szFolder, char* szFilename );
	~SkinnedMesh ( void );

	void UpdateAndRender ();
	void SetAnimationIndex ( int nIndex );
	void SetWorldMatrix(D3DXMATRIX* pMat) { controlMat = pMat; };
	void SetRandomTrackPosition (); // 테스트용
	void Moving();
	D3DXVECTOR3 GetCamBonePos();

	inline Bone* GetRoot() { return root; }
	inline void SetPosition ( D3DXVECTOR3 v )
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}

	inline D3DXVECTOR3 GetPosition()
	{
		return m_vPosition;
	}
	inline SphereInfo* GetBoundingSphere ()
	{
		return &m_stBoundingSphere;
	}

private:
	SkinnedMesh ();	

	void Load ( const char* foldername, const char* filename );
	LPD3DXEFFECT LoadEffect ( char* szFilename );
	void Update ( Bone* frame, D3DXMATRIX* pmatParent );
	void Render ( Bone* frame );
	void SetupBoneMatrixPtrs ( LPD3DXFRAME );

	void Destroy ();
	
private: 
	// 하나만 생성
	Bone*						root;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	SphereInfo					m_stBoundingSphere;
	D3DXMATRIX					worldMat;

	// 객체마다 생성
	D3DXMATRIX*					controlMat = nullptr;
	LPD3DXANIMATIONCONTROLLER	animationController;
	D3DXVECTOR3					m_vPosition;
	float						blendTime = 0.4f;
	float						passedBlendTime = 0.0f;
};










