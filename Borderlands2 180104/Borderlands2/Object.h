#pragma once

class Object
{
private:
	int refCount;

public:
	Object( void );
	virtual ~Object( void );

	virtual void AddRef();
	virtual void Release();
	virtual void AutoRelease();
};

