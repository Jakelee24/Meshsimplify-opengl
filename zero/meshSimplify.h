#ifndef MESHSIMPLIFY_H
#define MESHSIMPLIFY_H
#include "edgeHeap.h"
#include "vertexGroup.h"
#include "matrix.h"
#include "vector4.h"
#include "solve.h"
#include "config.h"
#include "model3d.h"
#include <cstdlib>
#include <cstdio>
#include <string>
using namespace std;
class MeshSimplify
{
	double ratio;
	int cntFace, cntDelFace;
	EdgeHeap* eHeap;
	VertexGroup* vGroup;
    //vector<int> vertexesOrder;
	vector<vector<int>>f_Group;
    struct OrderOfVertexes
    {
        vector<int> vertexesOrder;
    };
public:
	MeshSimplify(void);
	~MeshSimplify(void);

	void setRatio(double);
	
	
	void setLeftFacfeeNum(int);
	bool repetcheck(int *n, int *n2);
	void input(Model3d & model);
	void start();
	bool isrepeat(int * n, int * n2);
	void output(Model3d & model);

				  
	Matrix calVertexDelta(int);
	MyVec3d calVertexPos(Edge&, Matrix);
	
	void calVAndDeltaV(Edge&);	
};

#endif