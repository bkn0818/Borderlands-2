#include "stdafx.h"
#include "Picking.h"

//������
Picking::Picking()
{

}


//��ŷ ���� ��� - ��ũ�� �����͸� ����â �����ͷ� ��ȯ
Ray Picking::CalculatePickingRay(int x, int y, HWND hWnd)
{
	float fx = 0.0f;
	float fy = 0.0f;

	//����Ʈ ���
	D3DVIEWPORT9 dView;
	D3DDEVICE->GetViewport(&dView);

	//���� ���
	D3DXMATRIX mProj;
	D3DDEVICE->GetTransform(D3DTS_PROJECTION, &mProj);

	RECT rect;

	GetClientRect(hWnd, &rect);

	D3DXVECTOR3 vector;

	vector.x = ((((x - rect.left) * 2.0f / rect.right) - 1.0f) - mProj._31) / mProj._11;
	vector.y = (-(((y - rect.top) * 2.0f / rect.bottom) - 1.0f) - mProj._32) / mProj._22;
	vector.z = 1.0f;

	//�������
	D3DXMATRIXA16 matView, InverseView;
	D3DDEVICE->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&InverseView, NULL, &matView);

	//������ 3���� �������� �̵�
	this->ray.direction.x = vector.x * InverseView._11 + vector.y * InverseView._21 +
		vector.z * InverseView._31;
	this->ray.direction.y = vector.x * InverseView._12 + vector.y * InverseView._22 +
		vector.z * InverseView._32;
	this->ray.direction.z = vector.x * InverseView._13 + vector.y * InverseView._23 +
		vector.z * InverseView._33;

	this->ray.origin.x = InverseView._41;
	this->ray.origin.y = InverseView._42;
	this->ray.origin.z = InverseView._43;

	return this->ray;
}


//���� ��ȯ
void Picking::TransformRay(Ray* ray, D3DXMATRIX* T)
{
	//������ ��İ�
	D3DXVec3TransformCoord(&ray->origin, &ray->origin, T);

	//���⺤�Ϳ� ��İ�
	D3DXVec3TransformNormal(&ray->direction, &ray->direction, T);

	//D3DXVec3Normalize(&ray->direction, &ray->direction);
}


//������ ��ü�� �����ϴ��� �˻�
bool Picking::RaySphereIntTest(D3DXVECTOR3 Center, float Radian)
{
	//������ �������� ���� �߽����� ���� �Ÿ� ���ϱ�
	D3DXVECTOR3 vector = this->ray.origin - Center;

	//�������� ��ü���� �Ÿ��� ���� 2�� �������� �̿��ϱ� ���� a,b,c �� ���ϱ�
	float a = D3DXVec3Dot(&this->ray.direction, &this->ray.direction); //V^2
	float b = 2.0f * D3DXVec3Dot(&this->ray.direction, &vector); //2V * (S - C)
	float c = D3DXVec3Dot(&vector, &vector) - (Radian * Radian); //|S - C|^2 - R^2

																 //�Ǻ��� ���
	float discr = (b * b) - (4.0f * a * c);

	if (discr < 0.0f) //�Ǻ����� 0���� ������ ��ü�� �浹���� ����
	{
		return false;
	}
	else if (discr == 0) //�Ǻ����� 0�̸� �浹�� �ϳ�
	{
		//2�� ������ ���
		this->PickPos = this->ray.direction * (-b / (2.0f * a));

		return true;
	}
	else //�Ǻ����� 0���� ũ�� �浹���� 2��(�������� ����� ���� �浹������ �Ǵ�
	{
		//2�� ������ ���
		discr = sqrtf(discr);

		float s0 = (-b - discr) / (2.0f * a);
		float s1 = (-b + discr) / (2.0f * a);

		//�������� ����� �� ã��
		if (s0 < s1)
		{
			this->ray.fT = s0;
		}
		else
		{
			this->ray.fT = s1;
		}

		this->PickPos = this->ray.origin + this->ray.direction * this->ray.fT;

		return true;
	}

	return false;
}

//������ �ﰢ�� �浹 �˻�
bool Picking::RayTriangleIntTest(D3DXVECTOR3 &v0, D3DXVECTOR3 &v1, D3DXVECTOR3 &v2)
{
	D3DXVECTOR3 vectorP;
	D3DXVECTOR3 vecE1 = v1 - v0;
	D3DXVECTOR3 vecE2 = v2 - v0;

	D3DXVec3Cross(&vectorP, &this->ray.direction, &vecE2);

	float fDet = D3DXVec3Dot(&vecE1, &vectorP);

	if (fDet > -0.0001f && fDet < 0.0001f)
	{
		return false;
	}

	float fInvDet = 1.0f / fDet;

	D3DXVECTOR3 vectorS = this->ray.origin - v0;

	float u = fInvDet * D3DXVec3Dot(&vectorP, &vectorS);

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	D3DXVECTOR3 vectorQ;

	D3DXVec3Cross(&vectorQ, &vectorS, &vecE1);

	float v = fInvDet * D3DXVec3Dot(&vectorQ, &this->ray.direction);

	if (v < 0.0f || (u + v) > 1.0f)
	{
		return false;
	}

	this->ray.fT = fInvDet * D3DXVec3Dot(&vectorQ, &vecE2);

	if (this->ray.fT < 0.0f)
	{
		return false;
	}

	this->PickPos = this->ray.origin + this->ray.direction * this->ray.fT;

	return true;
}