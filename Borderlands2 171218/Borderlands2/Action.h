#pragma once

class Action;
class GameObject;

class iActionDelegate
{
public:
	virtual void OnActionFinish( Action* pSender ) = 0;
};

class Action : public Object
{

public:
	Action( void );
	virtual ~Action( void );

	virtual void Start();
	virtual void Update();


protected:
	float passedActionTime;

	GETSET( GameObject*, targetObject, TargetObject );
	GETSET( float, actionTime, ActionTime );
	GETSET( iActionDelegate*, actionDelegate, ActionDelegate );

};

