#pragma once

class iMap
{
public:
	iMap() {}
	virtual ~iMap() {}

	virtual HRESULT Load(char* szFullPath, D3DXMATRIX* pmat) = 0;
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};