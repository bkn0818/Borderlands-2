
#pragma once


class iMap
{
public:
	virtual void Load(char* szFullPath, D3DXMATRIX* pmat) = 0;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) = 0;
};

