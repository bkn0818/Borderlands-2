#ifndef PICKING_H
#define PICKING_H

#pragma once

struct Ray
{
	float fT;

	D3DXVECTOR3 origin;
	D3DXVECTOR3 direction;
};
class Picking
{
private:

public:
	Picking();
	Ray ray;
	D3DXVECTOR3 PickPos;
	Ray CalculatePickingRay(int x, int y, HWND hWnd);
	void TransformRay(Ray* ray, D3DXMATRIX* T);
	bool RaySphereIntTest(D3DXVECTOR3 center, float radian);
	bool RayTriangleIntTest(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2);
};

#endif // !PICKING_H