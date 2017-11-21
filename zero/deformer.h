#pragma once

#include "model3d.h"

class MeshSimplify;

class Deformer
{

public:
	struct PackedVertex {
		Vector3f position;
		//Vector2f uv;
		Vector3f normal;
		bool operator<(const PackedVertex that) const {
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
		};
	};
	Deformer();
	~Deformer();
	bool getSimilarVertexIndex_fast(PackedVertex & packed, map<PackedVertex, unsigned short>& VertexToOutIndex, unsigned short & result);
	void indexVBO(vector<Vector3f>& in_vertices, vector<Vector3f>& in_normals, vector<unsigned int>& out_indices, vector<GLfloat>& out_vertices, vector<GLfloat>& out_normals);
	void indexVBO(vector<Vector3f>& in_vertices, vector<Vector3f>& in_normals, vector<unsigned int>& out_indices, vector<Vector3f>& out_vertices, vector<Vector3f>& out_normals);
	vector<Vector3f> calnormal(vector<Vector3f>& vertexs, vector<unsigned int>& faces);
	void meshSimplify(Model3d & model, double ratio);
private:
	MeshSimplify *simplify;

};

