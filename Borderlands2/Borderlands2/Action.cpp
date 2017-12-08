#include "StdAfx.h"
#include "Action.h"

Action::Action( void )
	: passedActionTime( 0.0f )
	, targetObject( nullptr )
	, actionTime( 0.0f )
	, actionDelegate( nullptr )
{
}

Action::~Action( void )
{
}

void Action::Start()
{
	passedActionTime = 0.f;
}

void Action::Update()
{
}
