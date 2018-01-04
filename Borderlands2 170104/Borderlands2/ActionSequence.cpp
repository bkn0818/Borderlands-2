#include "StdAfx.h"
#include "ActionSequence.h"


ActionSequence::ActionSequence( void )
	: curIndex( 0 )
{
}

ActionSequence::~ActionSequence( void )
{
	for each( auto p in actionList )
	{
		SAFE_RELEASE( p );
	}
}

void ActionSequence::AddAction( Action* action )
{
	SAFE_ADDREF( action );	
	action->SetActionDelegate( this );
	actionList.push_back( action );
}

void ActionSequence::Start()
{
	if ( actionList.empty() )
		return;

	curIndex = 0;
	actionList[0]->Start();
}

void ActionSequence::Update()
{
	if ( curIndex < actionList.size() )
	{
		actionList[curIndex]->Update();
	}
}

void ActionSequence::OnActionFinish( Action* pSender )
{
	++curIndex;
	if ( curIndex >= actionList.size() )
	{
		if ( actionDelegate )
			actionDelegate->OnActionFinish( this );
	}
	else
	{
		actionList[curIndex]->Start();
	}
}
