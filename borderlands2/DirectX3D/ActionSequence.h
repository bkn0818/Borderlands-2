#pragma once

#include "Action.h"

//액션을 여러개 가지고 버퍼링으로 액션을 돌려줌
class ActionSequence : public Action, public iActionDelegate
{
protected:
	int						curIndex;
	std::vector<Action*>	actionList;

public:
	ActionSequence(void);
	virtual ~ActionSequence(void);

	virtual void AddAction(Action* action);

	// Action override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish(Action* pSender) override;
};

