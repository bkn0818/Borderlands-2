#include "StdAfx.h"
#include "ActionRepeat.h"


ActionRepeat::ActionRepeat( void )
	: action( nullptr )
{
}

ActionRepeat::~ActionRepeat( void )
{
	SAFE_RELEASE( action );
}

void ActionRepeat::Start()
{
	action->Start();
}

void ActionRepeat::Update()
{
	action->Update();
}

void ActionRepeat::OnActionFinish( Action* pSender )
{
	action->Start();
}
