#include "stdafx.h"
#include "PlayerCtrl.h"


PlayerCtrl::PlayerCtrl()
	:position(0,0,0)
{
	D3DXMatrixIdentity(&worldMatrix);
}


PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::Update(iMap * pMap)
{
	D3DXVECTOR3 tempPosition = position;
	D3DXVECTOR3 vDirection(0, 0, -1);

	if (pMap)
	{
		if (pMap->GetHeight(tempPosition.x, tempPosition.y, tempPosition.z))
		{
			position = tempPosition;
		}
	}
	else
	{
		position = tempPosition;
	}

	D3DXMATRIXA16 translationMatrix;
	D3DXMatrixTranslation(&translationMatrix, position.x, position.y, position.z);

	worldMatrix = translationMatrix;
}
