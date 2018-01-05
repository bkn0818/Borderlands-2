#pragma once

struct Bone;

class SkinnedMesh : public Object
{
	friend class SkinnedMeshManager;


public:
	SkinnedMesh(char* szFolder, char* szFilename);
	~SkinnedMesh(void);

	void UpdateAndRender();
	void SetAnimationIndex(int nIndex, bool isBlend = TRUE);
	void SetRandomTrackPosition(); // �׽�Ʈ��
	void Moving();
	void SetMoveEnable(bool value) { isMove = value; }
	D3DXVECTOR3 GetCamBonePos(LPCSTR str);
	D3DXMATRIX GetCamBoneMag05();
	bool GetAnimEnd(UINT animIndex);
	bool GetAnimHalf(UINT animIndex);
	LPD3DXMESH GetSphereMesh() { return mesh; }

	inline Bone* GetRoot() { return root; }
	inline void SetPosition(D3DXVECTOR3 v)
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}

	inline D3DXVECTOR3 GetPosition()
	{
		return m_vPosition;
	}
	inline SphereInfo* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}
	void	SetScale(float f) { if (f > 0.5f && f < 3.0f) scale = f; }
	float	GetScale() { return scale; }
	void	SetRotationY(float angle) { rotY = angle; }
	float	GetRotation() { return rotY; }

	inline void SetMoving(bool moving)
	{
		isMove = moving;
	}

	// ���� �߰�: skinnedMeshȸ����========== 
	inline void RotateY(float angle)
	{
		angleY = angle;
	}
	inline void RotateX(float angle)
	{
		angleX = angle;
	}
	// ======================================

private:
	SkinnedMesh();

	void Load(const char* foldername, const char* filename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(Bone* frame, D3DXMATRIX* pmatParent);
	void Blending();
	void Render(Bone* frame);
	void SetupBoneMatrixPtrs(LPD3DXFRAME);
	void DrawSphere();

	void Destroy();

private:
	// �ϳ��� ����
	Bone*						root;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	SphereInfo					m_stBoundingSphere;
	D3DXMATRIX					worldMat;
	LPD3DXMESH					mesh;
	float						scale = 1.0f;
	float						rotX = 1.0f, rotY = 1.0f;

	// ��ü���� ����
	LPD3DXANIMATIONCONTROLLER	animationController;
	D3DXVECTOR3					m_vPosition;
	float						blendTime = 0.2f;
	float						passedBlendTime = 0.0f;

	// ���� �߰�: skinnedMesh���� �÷��̾� sM�� �Բ� ȸ������ �ʰԲ� 
	float						angleX;
	float						angleY;
	bool						isMove;
	// =============================================================
};










