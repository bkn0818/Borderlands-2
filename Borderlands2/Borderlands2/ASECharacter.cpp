#include "stdafx.h"
#include "ASECharacter.h"

#include "ASEFrame.h"
#include "ASELoader.h"



ASECharacter::ASECharacter ( void )
	: root ( NULL )
{
}

ASECharacter::~ASECharacter ( void )
{
	root->Destroy ();
}

void ASECharacter::Load ( char* szFullPath )
{
	ASELoader* aseLoader = new ASELoader;
	root = aseLoader->Load ( szFullPath, &aseScene );
	root->AddRef ();
	SAFE_RELEASE ( aseLoader );
}

void ASECharacter::Update ()
{
	int firstTick = aseScene.firstFrame * aseScene.ticksPerFrame;
	int lastTick = aseScene.lastFrame * aseScene.ticksPerFrame;
	int tickSpeed = aseScene.frameSpeed * aseScene.ticksPerFrame;
	int keyFrame = ( ( int ) ( ( tickSpeed / 1000.f ) * GetTickCount () ) % ( lastTick - firstTick ) + firstTick );

	if ( root )
		root->Update ( keyFrame, NULL );
}

void ASECharacter::Render ()
{
	if ( root )
		root->Render ();
}
