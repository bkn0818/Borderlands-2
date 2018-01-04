#pragma once
#include "Enemy.h"

// bullymong과 다른 점: hp가 일정 수치 이하로 떨어지면 다른 bullymong 호출 
class Boss: public Enemy
{
public:
	Boss();
	~Boss();
};

