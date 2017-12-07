#pragma once
class ActionMove : public Action
{
protected:
	GETSET(D3DXVECTOR3, startPos, StartPos);
	GETSET(D3DXVECTOR3, targetPos, TargetPos);

public:
	ActionMove();
	virtual ~ActionMove();

	virtual void Start() override;
	virtual void Update() override;
};

