#include "model3d.h"



Model3d::Model3d(void)
{
}


Model3d::~Model3d(void)
{
}
void Model3d::ModelClear(V_Model &model)
{
	model.vertexFaces.clear();
	model.normalFaces.clear();
	model.uvFaces.clear();
	model.Indices.clear();
	model.vertexs.clear();
	model.normals.clear();
	model.uvs.clear();

}

bool Model3d::getSimilarVertexIndex_fast(
	PackedVertex3v &packed,
	map<PackedVertex3v, unsigned short> & VertexToOutIndex,
	unsigned short & result
) {
	map<PackedVertex3v, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

bool Model3d::getSimilarVertexIndex_fast(
	PackedVertex2v &packed,
	map<PackedVertex2v, unsigned short> & VertexToOutIndex,
	unsigned short & result
) {
	map<PackedVertex2v, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

void Model3d::indexVBO(
	vector<Vector3f> &in_vertices,
	vector<Vector2f> &in_uvs,
	vector<Vector3f> &in_normals,

	vector<unsigned int> &out_indices,
	vector<Vector3f> &out_vertices,
	vector<Vector2f> &out_uvs,
	vector<Vector3f> &out_normals
) {


	map<PackedVertex3v, unsigned short> VertexToOutIndex;

	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		PackedVertex3v packed = { in_vertices[i], in_uvs[i], in_normals[i] };


		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void Model3d::indexVBO(
	vector<Vector3f> &in_vertices,
	vector<Vector3f> &in_normals,

	vector<unsigned int> &out_indices,
	vector<Vector3f> &out_vertices,
	vector<Vector3f> &out_normals
) {
	map<PackedVertex2v, unsigned short> VertexToOutIndex;
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		PackedVertex2v packed = { in_vertices[i], in_normals[i] };


		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);

			out_normals.push_back(in_normals[i]);

			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}



}

void Model3d::indexVBO3v(V_Model &source_model, V_Model &out_model) 
{
	vector<Vector3f> temp_vertices;
	vector<Vector2f> temp_uvs;
	vector<Vector3f> temp_normals;
	for (unsigned int i = 0; i < source_model.vertexFaces.size(); i++)
	{
		unsigned int vertexIndex = source_model.vertexFaces[i];
		unsigned int uvIndex = source_model.uvFaces[i];
		unsigned int normalIndex = source_model.normalFaces[i];

		Vector3f vertex = source_model.vertexs[vertexIndex - 1];
		Vector2f uv = source_model.uvs[uvIndex - 1];
		Vector3f normal = source_model.normals[normalIndex - 1];
		temp_vertices.push_back(vertex);
		temp_normals.push_back(normal);
		temp_uvs.push_back(uv);


	}
	indexVBO(temp_vertices, temp_uvs, temp_normals,
		out_model.Indices,
		out_model.vertexs,
		out_model.uvs,
		out_model.normals
	);
}

void Model3d::indexVBO2v(V_Model &source_model, V_Model &out_model)
{
	vector<Vector3f> temp_vertices;
	vector<Vector3f> temp_normals;
	for (unsigned int i = 0; i < source_model.vertexFaces.size(); i++)
	{

		unsigned int vertexIndex = source_model.vertexFaces[i];
		unsigned int normalIndex = source_model.normalFaces[i];
		Vector3f vertex = source_model.vertexs[vertexIndex - 1];
		Vector3f normal = source_model.normals[normalIndex - 1];

		temp_vertices.push_back(vertex);
		temp_normals.push_back(normal);


	}

	ModelClear(out_model);
	indexVBO(
		temp_vertices,
		temp_normals,
		out_model.Indices,
		out_model.vertexs,
		out_model.normals
	);

}

void Model3d::calnormals(V_Model &model)
{
	model.normalFaces.clear();
	model.normals.clear();
	vector<Vector3f> sum_normals;
	for (unsigned int i = 0; i < model.vertexs.size(); i++) 
	{
		sum_normals.push_back(Vector3f(0.0f, 0.0f, 0.0f));
	}

	for (unsigned int i = 0; i < model.vertexFaces.size(); i += 3)
	{
		Vector3f va, vb, vc;
		unsigned int v_index, v1_index, v2_index;
		v_index = model.vertexFaces[i];
		v1_index = model.vertexFaces[i + 1];
		v2_index = model.vertexFaces[i + 2];

		va = model.vertexs[v1_index - 1] - model.vertexs[v_index - 1];
		vb = model.vertexs[v2_index - 1] - model.vertexs[v_index - 1];
		//cout << "Va: " << va[0] << " " << va[1] << " " << va[2] << endl;
		//cout << "Vb: " << vb[0] << " " << vb[1] << " " << vb[2] << endl;

		vc = vc.cross(va, vb);

		sum_normals[v_index - 1] += vc;
		sum_normals[v1_index - 1] += vc;
		sum_normals[v2_index - 1] += vc;
		//cout << "F: " << v_index << endl;
		//cout << "C:" << sum_normals[v_index - 1][0] << " " << sum_normals[v_index - 1][1] << " " << sum_normals[v_index - 1][2] << endl;
	}

	for (unsigned int i = 0; i < sum_normals.size(); i++)
	{

		GLfloat x, y, z, s;
		x = sum_normals[i][0];
		y = sum_normals[i][1];
		z = sum_normals[i][2];

		s = sqrtf(x*x + y*y + z*z);
		sum_normals[i][0] = x / s;
		sum_normals[i][1] = y / s;
		sum_normals[i][2] = z / s;

		//cout << "S:" << sum_normals[i][0] << " " << sum_normals[i][1] << " " << sum_normals[i][2] << endl;
	}
	sum_normals.swap(model.normals);
	model.normalFaces.assign(model.vertexFaces.begin(), model.vertexFaces.end());
}

void Model3d::load(const char* filename)
{
	vector<Vector3f> temp_vertices;
	vector<Vector2f> temp_uvs;
	vector<Vector3f> temp_normals;
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	
	ModelClear(this->V_Source_Model);
	//cout << this->V_Source_Model.vertexs.size() << endl;
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		cout << "Can't open the file !" << endl;
	}
	while (true) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   //else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			Vector3f vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			this->V_Source_Model.vertexs.push_back(vertex);
			
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vector2f texture;
			fscanf(file, "%f %f\n", &texture[0], &texture[1]);
			this->V_Source_Model.uvs.push_back(texture);
			
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vector3f normal;
			fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			this->V_Source_Model.normals.push_back(normal);
			
		}
		else if (strcmp(lineHeader, "f") == 0) 
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
				&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
				&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
			}

			this->V_Source_Model.vertexFaces.push_back(vertexIndex[0]);
			this->V_Source_Model.vertexFaces.push_back(vertexIndex[1]);
			this->V_Source_Model.vertexFaces.push_back(vertexIndex[2]);

			this->V_Source_Model.uvFaces.push_back(uvIndex[0]);
			this->V_Source_Model.uvFaces.push_back(uvIndex[1]);
			this->V_Source_Model.uvFaces.push_back(uvIndex[2]);

			this->V_Source_Model.normalFaces.push_back(normalIndex[0]);
			this->V_Source_Model.normalFaces.push_back(normalIndex[1]);
			this->V_Source_Model.normalFaces.push_back(normalIndex[2]);
			
		}
	}



}


void Model3d::VectorAssign(V_Model &cmodel, V_Model &vmodel)
{
	vmodel.normalFaces.assign(cmodel.normalFaces.begin(), cmodel.normalFaces.end());
	vmodel.normals.assign(cmodel.normals.begin(), cmodel.normals.end());
	vmodel.uvFaces.assign(cmodel.uvFaces.begin(), cmodel.uvFaces.end());
	vmodel.uvs.assign(cmodel.uvs.begin(), cmodel.uvs.end());
	vmodel.vertexFaces.assign(cmodel.vertexFaces.begin(), cmodel.vertexFaces.end());
	vmodel.vertexs.assign(cmodel.vertexs.begin(), cmodel.vertexs.end());
	vmodel.Indices.assign(cmodel.Indices.begin(), cmodel.Indices.end());
}


void Model3d::Copyto(Model3d &model)
{
	ModelClear(model.V_Source_Model);
	ModelClear(model.V_VBO_Model);

	VectorAssign(this->V_Source_Model, model.V_Source_Model);
	VectorAssign(this->V_VBO_Model, model.V_VBO_Model);
	
	

}

