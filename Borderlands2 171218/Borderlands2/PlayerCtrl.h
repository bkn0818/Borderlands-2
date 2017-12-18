#pragma once
class PlayerCtrl
{
public:
	PlayerCtrl();
	~PlayerCtrl();

	void Update(iMap* pMap = nullptr);

	D3DXVECTOR3* GetPosition() { return &position; };
	D3DXMATRIXA16* GetWorldTM() { return &worldMatrix; };

private:
	D3DXVECTOR3 position;
	D3DXMATRIXA16 worldMatrix;
};