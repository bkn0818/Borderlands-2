#pragma once

class CubeCtrl : public GameObject
{
public:
	CubeCtrl();
	~CubeCtrl();

	void Init(D3DXMATRIX& world);
	void Move(D3DXMATRIX& worldMatrix);
	void Render(D3DXMATRIX& worldMatrix);

	D3DXVECTOR3 GetPosition();

	// hierarchy animation
	void AddChild(CubeCtrl* cubeChild);
	void Destroy();
	void SetLocalTranslation(D3DXVECTOR3& v);
	void SetAngleSpeed(float fRotAngleSpeed);
	void SetPosition(D3DXVECTOR3 pos) { position = pos; }

private:
	D3DXMATRIX				matWorld;
	std::vector<VertexPT>	vertexPT;
	std::vector<VertexPNC>	indexList;
	D3DMATERIAL9			material;
	IDirect3DVertexBuffer9* vb;

	// hierarchy animation
	D3DXVECTOR3				localTranslation;
	std::vector<CubeCtrl*>	childList;
	float					rotationXAngle = 0.0f;
	float					rotationXAngleSpeed = 0.0f;
};

