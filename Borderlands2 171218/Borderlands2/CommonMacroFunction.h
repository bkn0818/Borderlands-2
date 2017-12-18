#pragma once

inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };

	return pt;
}

inline void SetWorldTransform(D3DXMATRIX pmat)
{
	D3DDEVICE->SetTransform(D3DTS_WORLD, &pmat);
}

inline void SetLighting(bool isLight)
{
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, isLight);
}

inline void SetWireFrame(bool isWire)
{
	D3DDEVICE->SetRenderState(D3DRS_FILLMODE, isWire);
}

inline DWORD FloatToDWORD(float f)
{
	return *((DWORD*)&f);
}

inline float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) return lowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

inline void GetRandomVector(D3DXVECTOR3* vOut, D3DXVECTOR3* vMin, D3DXVECTOR3* vMax)
{
	vOut->x = GetRandomFloat(vMin->x, vMax->x);
	vOut->y = GetRandomFloat(vMin->y, vMax->y);
	vOut->z = GetRandomFloat(vMin->z, vMax->z);
}

inline double GetDistance3D(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	return (float)sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}