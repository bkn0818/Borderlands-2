#pragma once

class Pyramid;

class GridCtrl
{
public:
	GridCtrl ();
	~GridCtrl ();

	void Init ();
	void Render ();


private:
	std::vector<VertexPC> vertexList;
	std::vector<Pyramid*> pyramidList;

};

