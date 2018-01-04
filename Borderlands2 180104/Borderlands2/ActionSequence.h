#pragma once

#include "Action.h"

class ActionSequence
	: public Action
	, public iActionDelegate
{
protected:
	int						curIndex;
	std::vector<Action*>	actionList;

public:
	ActionSequence( void );
	virtual ~ActionSequence( void );

	virtual void AddAction( Action* action );

	// Action override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish( Action* pSender ) override;
};

