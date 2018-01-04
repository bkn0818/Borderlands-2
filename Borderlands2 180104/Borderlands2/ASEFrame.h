#pragma once

#include "MatrialTexture.h"

class ASEFrame : public Object
{	
	friend class ASELoader;

public:
	ASEFrame ();
	~ASEFrame ();
	
	void Update ( int keyFrame, D3DXMATRIXA16* parent );
	void Render ();
	void AddChild ( ASEFrame* child );
	void Destroy ();
	void CalcLocalTM ( ASEFrame* pParent );
	void CalcLocalTranslationMatrix ( IN int nKeyFrame, OUT D3DXMATRIXA16& mat );
	void CalcLocalRotationMatrix ( IN int nKeyFrame, OUT D3DXMATRIXA16& mat );


protected:
	D3DXMATRIXA16				localTM;
	std::vector<ASEFrame*>		childList;

	std::vector<PositionSample>	posTrackList;
	std::vector<RotationSample>	rotTrackList;


	GETSET_REF ( D3DXMATRIXA16, worldTM, WorldTM );
	GETSET_REF ( std::vector<VertexPNT>, PNTVerts, PNTVerts );
	GETSET_ADD_REF ( MaterialTexture*, materialTexture, MaterialTexture );


};

