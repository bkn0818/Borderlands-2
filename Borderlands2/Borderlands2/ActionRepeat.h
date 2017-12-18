#pragma once

#include "Action.h"

class ActionRepeat
	: public Action
	, public iActionDelegate
{
protected:
	GETSET_ADD_REF( Action*, action, Action );

public:
	ActionRepeat( void );
	virtual ~ActionRepeat( void );

	// cAction override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish( Action* pSender ) override;
};

