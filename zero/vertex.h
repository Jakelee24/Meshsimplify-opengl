#ifndef VERTEX_H
#define VERTEX_H
#include "config.h"
#include "myVec3d.h"
#include <vector>
#include <set>

class Vertex
{
public:
	Vertex(void);
	Vertex(MyVec3d);
	Vertex(double, double, double);
	~Vertex(void);

	int id;
	MyVec3d pos;
	std::vector<int> vertexesOrder;
	void addConnectVertex(int);
    void addOrderOfVertexes (int num);
    std::set<int> connectVertexes;
	void delConnectVertex(int);
	bool isExistConnectVertex(int);
};
#endif 