#pragma once

//�׼� ����
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

