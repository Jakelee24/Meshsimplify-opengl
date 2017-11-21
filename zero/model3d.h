#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "vecmath.h"
#include "edgeHeap.h"
#include "vertexGroup.h"
#include "matrix.h"
#include "vector4.h"
#include "solve.h"
#include "config.h"
#include "Model3d.h"
class Model3d
{
public:
	
	struct PackedVertex3v {
		Vector3f position;
		Vector2f uv;
		Vector3f normal;
		bool operator<(const PackedVertex3v that) const {
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex3v))>0;
		};
	};
	struct PackedVertex2v {
		Vector3f position;
		Vector3f normal;
		bool operator<(const PackedVertex2v that) const {
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex2v))>0;
		};
	};
	struct V_Model
	{
		vector<Vector3f> vertexs;
		vector<Vector3f> normals;
		vector<Vector2f> uvs;
		vector <unsigned int> vertexFaces;
		vector <unsigned int> uvFaces;
		vector <unsigned int> normalFaces;
		vector<unsigned int> Indices;
		
	};
	V_Model V_Source_Model;
	V_Model V_VBO_Model;

	
	Model3d(void);
	~Model3d(void);
	void load(const char* filename);
	void Copyto(Model3d & model);
	void ModelClear(V_Model &model);
	void indexVBO(
		vector<Vector3f> &in_vertices,
		vector<Vector2f> &in_uvs,
		vector<Vector3f> &in_normals,

		vector<unsigned int> &out_indices,
		vector<GLfloat> &out_vertices,
		vector<GLfloat> &out_uvs,
		vector<GLfloat> &out_normals
	);
	

	void indexVBO3v(V_Model & source_model, V_Model & out_model);

	void indexVBO2v(V_Model & source_model, V_Model & out_model);

	void calnormals(V_Model & model);

	
	
private:
	bool getSimilarVertexIndex_fast(PackedVertex3v & packed, map<PackedVertex3v, unsigned short>& VertexToOutIndex, unsigned short & result);
	bool getSimilarVertexIndex_fast(PackedVertex2v & packed, map<PackedVertex2v, unsigned short>& VertexToOutIndex, unsigned short & result);
	void indexVBO(
		vector<Vector3f> &in_vertices,
		vector<Vector2f> &in_uvs,
		vector<Vector3f> &in_normals,

		vector<unsigned int> &out_indices,
		vector<Vector3f> &out_vertices,
		vector<Vector2f> &out_uvs,
		vector<Vector3f> &out_normals
	);

	void indexVBO(vector<Vector3f>& in_vertices, vector<Vector3f>& in_normals, vector<unsigned int>& out_indices, vector<Vector3f>& out_vertices, vector<Vector3f>& out_normals);
	
	void VectorAssign(V_Model & cmodel, V_Model & vmodel);

};

