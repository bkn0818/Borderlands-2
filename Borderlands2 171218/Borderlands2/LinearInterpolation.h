#pragma once

class cCube;

class LinearInterpolation : public Object
{
private:
	std::vector<VertexPC>	lineVerts;
	CubeCtrl*				cubeCtrl;

public:
	LinearInterpolation( void );
	virtual ~LinearInterpolation( void );

	void Init();
	void Update();
	void Render();
};

