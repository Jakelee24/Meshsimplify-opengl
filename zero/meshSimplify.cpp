#include "meshSimplify.h"
MeshSimplify::MeshSimplify(void)
{
	eHeap = new EdgeHeap();
	vGroup = new VertexGroup();
	cntFace = 0;
}


MeshSimplify::~MeshSimplify(void)
{
}

void MeshSimplify::start() {
	for (int i = 0; i < cntDelFace; i += 2) {
		//Get deleted two point from edge
		Edge e = eHeap->getMinDelta();
		Vertex* v1 = &(vGroup->group[e.v1]);
		Vertex* v2 = &(vGroup->group[e.v2]);
		
		//New position of new point
		Vertex v0 = e.v;
		//Add new point to the group and count of vertex plus one
		int v0_id = vGroup->addVertex(v0);
		Vertex* pV0 = &(vGroup->group[v0_id]);
		//Init face list for new point
		vector<int> n_G;
        Vertex j_G;
		f_Group.push_back(n_G);
		set<int> connectV;
		connectV.clear();
		eHeap->delEdge(e);
        
		//Point one
		for (set<int>::iterator it = v1->connectVertexes.begin(); 
            it != v1->connectVertexes.end(); it++) {
			if ((*it) != v2->id) {
				//Delete all point one's edge
				eHeap->delEdge(Edge((*it), v1->id));
				//Delete the connect from point one
				vGroup->group[(*it)].delConnectVertex(v1->id);
				//Insert the point to the list which is connected point one
				connectV.insert((*it));
				
                for (unsigned int i = 0; i < vGroup->group[*it].vertexesOrder.size (); i++)
                {
                    if (vGroup->group[*it].vertexesOrder[i] == v1->id)
                        vGroup->group[*it].vertexesOrder[i] = -1;
                }

			}

		}

		for (set<int>::iterator it = v2->connectVertexes.begin(); 
            it != v2->connectVertexes.end(); it++) {
			if ((*it) != v1->id) {
				//Delete all point two's edge
				eHeap->delEdge(Edge((*it), v2->id));
				//Delete the connect from point two
				vGroup->group[(*it)].delConnectVertex(v2->id);
				//Insert the point to the list which is connected point two
				connectV.insert((*it));
				
                for (unsigned int i = 0; i < vGroup->group[*it].vertexesOrder.size (); i++)
                {
                    if (vGroup->group[*it].vertexesOrder[i] == v2->id)
                        vGroup->group[*it].vertexesOrder[i] = -1;
                }
			}
		}

        vector<int> tmp_t;
		for (set<int>::iterator it = connectV.begin(); it != connectV.end(); it++) {
            tmp_t.clear ();
           
            for (unsigned int i = 0; 
                i < vGroup->group[*it].vertexesOrder.size (); 
                i++)
            {
                if (vGroup->group[*it].vertexesOrder[i] == -1)
                {
                    vGroup->group[*it].vertexesOrder[i] = v0_id;
                    tmp_t.push_back (i);
                }
            }
				
			vGroup->group[(*it)].addConnectVertex(v0_id);
			vGroup->group[v0_id].addConnectVertex(*it);	

            for (unsigned int i = 0; i < tmp_t.size (); i++)
            {
                if (tmp_t[i] % 2 == 1)
                {
                    vGroup->group[v0_id].vertexesOrder.push_back (*it);
                    vGroup->group[v0_id].vertexesOrder.push_back (
                        vGroup->group[*it].vertexesOrder[tmp_t[i] - 1]
                    );
                }
                else
                {
                    vGroup->group[v0_id].vertexesOrder.push_back (
                        vGroup->group[*it].vertexesOrder[tmp_t[i] + 1]
                    );
                    vGroup->group[v0_id].vertexesOrder.push_back (*it);
                }
            }
		}

        for (unsigned int i = 0; 
            i < vGroup->group[v0_id].vertexesOrder.size (); 
            i += 2)
        {
            int f[2];
            f[0] = vGroup->group[v0_id].vertexesOrder[i];
            f[1] = vGroup->group[v0_id].vertexesOrder[i + 1];

            if (f[0] == f[1])
            {
                vGroup->group[v0_id].vertexesOrder.erase (
                    vGroup->group[v0_id].vertexesOrder.begin () + i,
                    vGroup->group[v0_id].vertexesOrder.begin () + i + 1
                );
            }
        }
		vGroup->isDeleted[v1->id] = true;
		vGroup->isDeleted[v2->id] = true;

		for (set<int>::iterator it = connectV.begin(); it != connectV.end(); it++) {
			Edge e((*it), v0_id);
			calVAndDeltaV(e);
			eHeap->addEdge(e);
		}

	}
	
}

void MeshSimplify::setRatio(double _ratio) {
	ratio = _ratio;
}

void MeshSimplify::input(Model3d &model) {
	int cntv = 0, cntf = 0;

	for (unsigned int i = 0; i < model.V_Source_Model.vertexs.size(); i++)
	{
		cntv++;
		double x, y, z;
		x = (double)model.V_Source_Model.vertexs[i][0];
		y = (double)model.V_Source_Model.vertexs[i][1];
		z = (double)model.V_Source_Model.vertexs[i][2];
		
		vGroup->addVertex(Vertex(x, y, z));
	}
	vector<int> n_G;
	
	for (unsigned int i = 0; i <= model.V_Source_Model.vertexs.size(); i++)
	{
		f_Group.push_back(n_G);
	}
	
	
	for (unsigned int i = 0; i < model.V_Source_Model.vertexFaces.size(); i+=3)
	{
		cntFace++;
		cntf++;
		int a, b, c;
		
		a = model.V_Source_Model.vertexFaces[i];
		b = model.V_Source_Model.vertexFaces[i + 1];
		c = model.V_Source_Model.vertexFaces[i + 2];

		vGroup->group[a].addConnectVertex(b);
		vGroup->group[a].addConnectVertex(c);
		vGroup->group[b].addConnectVertex(c);
		vGroup->group[b].addConnectVertex(a);
		vGroup->group[c].addConnectVertex(a);
		vGroup->group[c].addConnectVertex(b);

        vGroup->group[a].addOrderOfVertexes (b);
        vGroup->group[a].addOrderOfVertexes (c);
        vGroup->group[b].addOrderOfVertexes (c);
        vGroup->group[b].addOrderOfVertexes (a);
        vGroup->group[c].addOrderOfVertexes (a);
        vGroup->group[c].addOrderOfVertexes (b);

	}
	
	for (int i = 1; i <= vGroup->cntVertex; i++) {
		for (set<int>::iterator it = vGroup->group[i].connectVertexes.begin();
			it != vGroup->group[i].connectVertexes.end(); it++) {
			if (i < (*it))
				break;

			Edge e((*it), i);
			calVAndDeltaV(e);
			eHeap->addEdge(e);
			
		}
	}
	cntDelFace = (int)((1 - ratio) * cntFace);

}

bool MeshSimplify::isrepeat(int *n, int *n2)
{
	int c_n[3], c_n2[3];
	
	for (int i = 0; i < 3; i++) 
	{
		c_n[i] = n[i];
		c_n2[i] = n2[i];
	}
	sort(c_n, c_n + 3);
	sort(c_n2, c_n2 + 3);
	
	for (int i = 0; i < 3; i++)
	{
		if (c_n[i] != c_n2[i])
			return false;
	}

	return true;
}
void MeshSimplify::output(Model3d &model) {
	int cnt = 0, fcnt = 0;
	int cntv = 0, cntf = 0;
	int counter = 0;
	vector<int> isDelete;
    vector<Vertex> temp_OrderofVertexs;
    
    for (unsigned int i = 0; i <= vGroup->cntVertex; i++)
    {
        temp_OrderofVertexs.push_back (vGroup->group[i]);
    }

	for (int i = 1; i <= vGroup->cntVertex; i++) {
		if (vGroup->isDeleted[i])
		{
            for (unsigned int a = 1; a < temp_OrderofVertexs.size(); a++)
            {
                for (unsigned int b = 0;
                    b < temp_OrderofVertexs[a].vertexesOrder.size();
                    b++) {

                    if (vGroup->group[a].vertexesOrder[b] > i)
                    {
                        temp_OrderofVertexs[a].vertexesOrder[b]--;
                    }
                }
            }
			isDelete.push_back(i);
			counter++;
			continue;
		}

		Vertex* v = &vGroup->group[i];
		cnt++;

		v->id = cnt;
		model.V_VBO_Model.vertexs.push_back(Vector3f(v->pos.x, v->pos.y, v->pos.z));
	}
    for (unsigned int i = 0; i <= vGroup->cntVertex; i++)
    {
        vGroup->group[i].vertexesOrder.clear ();
    }

    for (unsigned int i = 0; i <= vGroup->cntVertex; i++)
    {
        temp_OrderofVertexs[i].vertexesOrder.swap (
            vGroup->group[i].vertexesOrder
        );
        temp_OrderofVertexs[i].vertexesOrder.clear ();
    }

    int delcnt = 0;
    int cntswap = 1;
    for (unsigned int i = 1; i <= vGroup->cntVertex; i++)
    {
        if (isDelete.size () != 0 && i == isDelete[delcnt])
        {
            vGroup->group[i].vertexesOrder.clear ();
            delcnt++;
            continue;
        }

        vGroup->group[i].vertexesOrder.swap (
            temp_OrderofVertexs[cntswap].vertexesOrder
        );
        vGroup->group[i].vertexesOrder.clear ();
        cntswap++;
    }
    

    for (unsigned int i = 1; i < cntswap; i++)
    {
        temp_OrderofVertexs[i].vertexesOrder.swap (
            vGroup->group[i].vertexesOrder
        );

    }

    
    for (unsigned int i = 1; i < cntswap; i++)
    {
        for (unsigned int j = 0; 
            j < vGroup->group[i].vertexesOrder.size (); 
            j++)
        {
            if (vGroup->group[i].vertexesOrder[j] == i)
            {
                for (unsigned int k = j;
                    k < vGroup->group[i].vertexesOrder.size ()-1;
                    k++)
                {
                    vGroup->group[i].vertexesOrder[k] = vGroup->group[i].vertexesOrder[k + 1];
                }
            }
        }
        vGroup->group[i].vertexesOrder.erase (
            vGroup->group[i].vertexesOrder.begin () +
            vGroup->group[i].vertexesOrder.size () - 1
        );
    }


	for (int i = 1; i < vGroup->cntVertex; i++) {

		if (vGroup->isDeleted[i])
			continue;
		Vertex* v = &(vGroup->group[i]);

		for (set<int>::iterator it1 = v->connectVertexes.begin(); 
            it1 != v->connectVertexes.end(); 
            it1++) {

			if (i >= (*it1))
				continue;
			for (set<int>::iterator it2 = v->connectVertexes.begin(); 
                it2 != v->connectVertexes.end(); 
                it2++) {

				if ((*it1) < (*it2) && (vGroup->group[(*it1)].isExistConnectVertex(*it2))) {
				    int f[3];
					int c_f[3];
					f[0] = v->id;
					f[1] = vGroup->group[(*it1)].id;
					f[2] = vGroup->group[(*it2)].id;
					//cout << "v_id: " << v->id << endl;
					//cout << "f: " << f[0] << " " << f[1] << " " << f[2] << endl;
					int r_cnt = 0;
					for (unsigned int k = 0; k < cntswap; k+=2)
					{
                        c_f[0] = v->id;
                        c_f[1] = vGroup->group[v->id].vertexesOrder[k];
                        c_f[2] = vGroup->group[v->id].vertexesOrder[k + 1];
						if (isrepeat(c_f, f))
						{
							r_cnt++;
							break;
						}
					}
					//cout << "c_f: " << c_f[0] << " " << c_f[1] << " " << c_f[2] << endl;
					if (r_cnt == 0) {
						model.V_VBO_Model.vertexFaces.push_back(f[0]);
						model.V_VBO_Model.vertexFaces.push_back(f[1]);
						model.V_VBO_Model.vertexFaces.push_back(f[2]);
					}
					else
					{
						model.V_VBO_Model.vertexFaces.push_back(c_f[0]);
						model.V_VBO_Model.vertexFaces.push_back(c_f[1]);
						model.V_VBO_Model.vertexFaces.push_back(c_f[2]);
					}
					cntf++;
				}
			}
		}
	}
	
}

void MeshSimplify::calVAndDeltaV(Edge& e) {
	Matrix mat = calVertexDelta(e.v1) + calVertexDelta(e.v2);
	e.v = calVertexPos(e, mat);	
	Vector4 X(e.v.x, e.v.y, e.v.z, 1.0);
	if (vGroup->getCommonVertexNum(e.v1, e.v2) != 2) {
		e.deltaV = 0;
		return;
	}
	double pri = 0;
	for (int i = 0; i < 4; i++) {
		double p = 0;
		for (int j = 0; j < 4; j++)
			p += X.v[j] * mat.mat[i][j];
		pri += p * X.v[i];
	}
	e.deltaV = pri;
}

Matrix MeshSimplify::calVertexDelta(int _id) {
	Matrix ans;
	Vertex* p = &(vGroup->group[_id]);
	for (set<int>::iterator it1 = p->connectVertexes.begin(); it1 != p->connectVertexes.end(); it1++) {
		for (set<int>::iterator it2 = p->connectVertexes.begin(); it2 != p->connectVertexes.end(); it2++) {
			if ((*it1) < (*it2) && (vGroup->group[(*it1)].isExistConnectVertex(*it2))) {
				Vertex* v1 = &(vGroup->group[(*it1)]);
				Vertex* v2 = &(vGroup->group[(*it2)]);
				MyVec3d n = ((v1->pos) - (p->pos)).getCross((v2->pos) - (p->pos)).getUnitVectorOfThis();
				Vector4 tmp(n.x, n.y, n.z, -(p->pos.getDot(n)));
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						ans.mat[i][j] += tmp.v[i] * tmp.v[j];
					}

				}

			}

		}

	}
	return ans;

}

MyVec3d MeshSimplify::calVertexPos(Edge& e, Matrix m) {
	MyVec3d mid = (vGroup->group[e.v1].pos + vGroup->group[e.v2].pos) / 2;  
	m.mat[3][0] = 0;
	m.mat[3][1] = 0;
	m.mat[3][2] = 0;
	m.mat[3][3] = 1;

	Vector4 Y(0, 0, 0, 1);
	Solve* solve = new Solve(m, Y);
	Vector4 ans = solve->getAns();
	if (ans.v[3] > Config::EPS)
		return MyVec3d(ans.v[0], ans.v[1], ans.v[2]);
	else
		return mid;
}
