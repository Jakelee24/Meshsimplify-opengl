#ifndef EDGE_H
#define EDGE_H
#include "myVec3d.h"
#include <cstdlib>
#include <cstdio>

class Edge
{
public:
	Edge(int _u = -99, int _v = -99);
	~Edge(void);
	int id;
	int v1, v2;
	MyVec3d v;
	double deltaV;
};

#endif