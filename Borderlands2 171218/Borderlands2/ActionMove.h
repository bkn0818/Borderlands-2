#pragma once

#include "Action.h"

class ActionMove : public Action
{
public:
	ActionMove( void );
	virtual ~ActionMove( void );

	// cAction override
	virtual void Start() override;
	virtual void Update() override;


protected:
	GETSET( D3DXVECTOR3, startPosition, StartPosition );
	GETSET( D3DXVECTOR3, targetPosition, TargetPosition );

};

