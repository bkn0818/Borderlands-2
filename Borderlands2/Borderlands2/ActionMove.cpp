#include "StdAfx.h"
#include "ActionMove.h"


ActionMove::ActionMove( void )
	: startPosition( 0, 0, 0 )
	, targetPosition( 0, 0, 0 )
{
}

ActionMove::~ActionMove( void )
{
}

void ActionMove::Start()
{
	if ( !targetObject )
		return;

	targetObject->SetPosition( startPosition );
	passedActionTime = 0.0f;
}

void ActionMove::Update()
{
	if ( !targetObject )
		return;

	passedActionTime += g_pTimeManager->GetDeltaTime();
	if ( passedActionTime < actionTime )
	{
		float t = passedActionTime / actionTime;
		D3DXVECTOR3 p;
		D3DXVec3Lerp( &p, &startPosition, &targetPosition, t );
		targetObject->SetPosition( p );
	}
	else
	{
		if ( actionDelegate )
		{
			actionDelegate->OnActionFinish( this );
		}
	}
}
