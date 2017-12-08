#pragma once

#include "Action.h"

class GameObject : public Object
{

public:
	GameObject( void );
	virtual ~GameObject( void );
	virtual void Update();
	virtual void Render();

protected:
	D3DXMATRIXA16	worldMatrix;
	D3DXVECTOR3		prevPosition;

	GETSET ( Action*, action, Action );
	GETSET_REF( D3DXVECTOR3, direction, Direction );
	GETSET_REF( D3DXVECTOR3, position, Position );

};

