#pragma once
class CharacterCtrl
{
public:
	CharacterCtrl();
	~CharacterCtrl();

	void Init();
	void Update(iMap* obj);
	void GetCameraLookAt(D3DXVECTOR3& lookAt, D3DXVECTOR3& side);
	D3DXVECTOR3& GetPosition();
	D3DXMATRIX& GetWorldTM();

	GETSET(D3DXVECTOR3, positionVector, PositionVector);

private:
	D3DXMATRIX worldMatrix;
	float rotationY;
	float speed;
	D3DXVECTOR3 vMoveToFoward;
	D3DXVECTOR3 vMoveToSide;


};

