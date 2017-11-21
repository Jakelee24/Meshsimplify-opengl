#ifndef VERTEXGROUP_H
#define VERTEXGROUP_H
#include "vertex.h"
#include "config.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class VertexGroup
{
public:
	VertexGroup(void);
	~VertexGroup(void);
	Vertex group[Config::MAX_POINT_NUM];
	int cntVertex;
    int cntOrderOfVertex;
	bool* isDeleted;
	int addVertex(Vertex);
	void delVertex(int);
	int getCommonVertexNum(int, int);
};
#endif