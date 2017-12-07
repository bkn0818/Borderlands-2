#pragma once

//액션 관리
class LinearInterpolation
{
private:
	CubeCtrl* cube;
	std::vector<VertexPC> vertexs;

public:
	LinearInterpolation();
	~LinearInterpolation();

	void Init();
	void Update();
	void Render();
};

