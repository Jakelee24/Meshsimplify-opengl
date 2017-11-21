#include "deformer.h"
#include "meshSimplify.h"


Deformer::Deformer()
{
	simplify = new MeshSimplify();
}


Deformer::~Deformer()
{
	delete simplify;
}



void Deformer::meshSimplify(Model3d & model, double ratio)
{
	vector<Vector3f> in_vertices;
	vector<Vector2f> in_uvs;
	vector<Vector3f> in_normals;
	model.ModelClear(model.V_VBO_Model);
	
	simplify->setRatio(ratio);
	simplify->input(model);
	simplify->start();
	simplify->output(model);
	model.calnormals(model.V_VBO_Model);
}