#pragma once

class MaterialTexture;
class ASEFrame;

class ASELoader : public Object
{
public:
	ASELoader ( void );
	virtual ~ASELoader ( void );

	ASEFrame*	Load ( IN char* filename, OUT ASEScene* pScene );
	char*		GetToken ();
	int			GetInteger ();
	float		GetFloat ();
	bool		IsWhite ( char c );
	bool		IsEqual ( char* str1, char* str2 );
	void		SkipBlock ();
	void		ProcessMATERIAL_LIST ();
	void		ProcessMATERIAL ( OUT MaterialTexture* pMtlTex );
	void		ProcessMAP_DIFFUSE ( OUT MaterialTexture* pMtlTex );
	ASEFrame*	ProcessGEOMOBJECT ();
	void		ProcessMESH ( OUT ASEFrame* pFrame );
	void		ProcessMESH_VERTEX_LIST ( OUT std::vector<D3DXVECTOR3>& vecV );
	void		ProcessMESH_FACE_LIST ( IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<VertexPNT>&	vecVertex );
	void		ProcessMESH_TVERTLIST ( OUT std::vector<D3DXVECTOR2>& vecVT );
	void		ProcessMESH_TFACELIST ( IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<VertexPNT>&	vecVertex );
	void		ProcessMESH_NORMALS ( OUT std::vector<VertexPNT>&	vecVertex );
	void		ProcessNODE_TM ( OUT ASEFrame* pFrame );
	void		ProcessTM_ANIMATION ( OUT ASEFrame* pFrame );
	void		ProcessPOS_TRACK ( OUT ASEFrame* pFrame );
	void		ProcessROT_TRACK ( OUT ASEFrame* pFrame );
	void		ProcessSCENE ( OUT ASEScene* pScene );


protected:
	FILE*								fileStream;
	char								m_szToken[1024];
	std::vector<MaterialTexture*>		materialTextureList;
	std::map<std::string, ASEFrame*>	frameMap;
	ASEFrame*							root;

};

