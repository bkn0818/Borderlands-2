#pragma once

#include "iMap.h"


class OBJMap : public iMap
{
protected:
	std::vector<D3DXVECTOR3> m_vecVertex;

public:
	OBJMap ( void );
	virtual ~OBJMap ( void );

	virtual void Load ( char* szFullPath, D3DXMATRIX* pmat ) override;
	virtual bool GetHeight ( IN const float& x, OUT float& y, IN const float& z ) override;
};

