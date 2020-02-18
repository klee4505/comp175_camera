#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	vector< vector<vertex> > vertex_list_list;
	vector< vector<triangle> > triangle_list_list;

	float size = 1;
	Sphere() {
		setupShape();
	};
	~Sphere() {};

	OBJ_TYPE getType() {
		return SHAPE_SPHERE;
	}

	void setupShape() {
		// Resetting lists of vertices and triangles each time
		for (int v_list = 0; v_list < vertex_list_list.size(); v_list++) {
			vertex_list_list[v_list].clear();
		}
		for (int t_list = 0; t_list < triangle_list_list.size(); t_list++) {
			triangle_list_list[t_list].clear();
		}
		vertex_list_list.clear();
		triangle_list_list.clear();

		gen_vertices_faces();
	}

	void gen_vertices_faces() {
		float start_x = 0;
		float start_y = -0.5;
		float start_z = 0;
		float thetaX = 2 * PI / m_segmentsX;
		float phiY = PI / m_segmentsY;
		vector<vertex> top_vertex_list;
		vector<vertex> bot_vertex_list;
		vector<vertex> side_vertex_list;
		vertex v;
		vector<float> layer_rads;

		for (int y = 0; y < m_segmentsY + 1; y++) {
			layer_rads.push_back(sin(phiY * y) * size / 2);
		}

		for (int x = 0; x < m_segmentsX; x++) {
			// Bot -- Note: builds in clock-wise direction
			v.x = layer_rads[1] * cos(x * thetaX);
			v.z = layer_rads[1] * sin(x * thetaX);
			v.y = size / 2 * cos(phiY * (m_segmentsY - 1));
			bot_vertex_list.push_back(v);

			//Side
//			v = bot_vertex_list[x];
			side_vertex_list.push_back(v);

			//Goes from 2nd layer to last layer (first layer done by Bot, last layer done for Top)
			for (int y = 2; y < m_segmentsY; y++) {
				v.y = size / 2 * cos(phiY * (m_segmentsY - y));
				v.x = layer_rads[y] * cos(x * thetaX);
				v.z = layer_rads[y] * sin(x * thetaX);
				side_vertex_list.push_back(v);
			}
			top_vertex_list.push_back(v);
			// Wrap-around: sides collectively treated like a wrapped-up rectangle
			/* Order of filling:
			   Eg. for m_segmentsX = 3, m_segmentsY = 4:
				[[(4) 14 9 4]
				 [(3) 13 8 3]
				 [(2) 12 7 2]
				 [(1) 11 6 1]
				 [(0) 10 5 0]]
			*/
		}			
		v.x = start_x;
		v.y = start_y;
		v.z = start_z;
		bot_vertex_list.push_back(v);
		v.y = -start_y;
		top_vertex_list.push_back(v);

		vertex_list_list.push_back(bot_vertex_list);
		vertex_list_list.push_back(top_vertex_list);
		vertex_list_list.push_back(side_vertex_list);

		vector<triangle> bot_tri_list;
		vector<triangle> top_tri_list;
		vector<triangle> side_tri_list;
		triangle t;
		for (int x = 0; x < m_segmentsX; x++) {
			// Bot/Top
			t.v_idx_list[0] = m_segmentsX;
			t.v_idx_list[1] = x;
			t.v_idx_list[2] = (x + 1) % m_segmentsX;
			bot_tri_list.push_back(t);
			t.v_idx_list[1] = t.v_idx_list[2];
			t.v_idx_list[2] = x;
			top_tri_list.push_back(t);

			// Sides
			int num_side_vertices = m_segmentsX * (m_segmentsY - 1);
			int side_height = m_segmentsY - 1;
			for (int y = 0; y < side_height - 1; y++) {
				// "Upper" triangle
				t.v_idx_list[0] = y + x * (side_height);
				t.v_idx_list[1] = (t.v_idx_list[0] + side_height + 1) % num_side_vertices;
				t.v_idx_list[2] = t.v_idx_list[1] - 1;
				side_tri_list.push_back(t);

				// "Lower" triangle
				t.v_idx_list[2] = t.v_idx_list[1];
				t.v_idx_list[1] = t.v_idx_list[0] + 1;
				side_tri_list.push_back(t);
			}
		}
		triangle_list_list.push_back(bot_tri_list);
		triangle_list_list.push_back(top_tri_list);
		triangle_list_list.push_back(side_tri_list);
	}

	void draw() {
		if (vertex_list_list.size() == 0 || triangle_list_list.size() == 0 || vertex_list_list[0].size() == 0 || triangle_list_list[0].size() == 0) {
			cerr << "Vertex list or triangle list(s) empty." << endl;
			return;
		}
		int v_count = 3; // triangle_list_list[0][0].v_count
		int num_areas = triangle_list_list.size(); //@TODO: should be 2
		int num_tris;

		for (int area = 0; area < num_areas; area++) {
			float xs[3];
			float ys[3];
			float zs[3];

			num_tris = triangle_list_list[area].size();
			for (int tri = 0; tri < num_tris; tri++) {
				for (int v_idx = 0; v_idx < v_count; v_idx++) {
					xs[v_idx] = vertex_list_list[area][triangle_list_list[area][tri].v_idx_list[v_idx]].x;
					ys[v_idx] = vertex_list_list[area][triangle_list_list[area][tri].v_idx_list[v_idx]].y;
					zs[v_idx] = vertex_list_list[area][triangle_list_list[area][tri].v_idx_list[v_idx]].z;
				}
				glBegin(GL_TRIANGLES);
				setNormal(xs[0], ys[0], zs[0], xs[1], ys[1], zs[1], xs[2], ys[2], zs[2]);
				glVertex3f(xs[0], ys[0], zs[0]);
				glVertex3f(xs[1], ys[1], zs[1]);
				glVertex3f(xs[2], ys[2], zs[2]);
				glEnd();
			}
		}
	};

	void drawNormal() {
		if (vertex_list_list.size() == 0) {
			return;
		}
		int num_faces = vertex_list_list.size();

		for (int face = 0; face < num_faces; face++) {
			int num_points = vertex_list_list[face].size();
			for (int i = 0; i < num_points; i++) {
				vertex v = vertex_list_list[face][i];
				glBegin(GL_LINES);
				glVertex3f(v.x, v.y, v.z); glVertex3f(1.1*v.x, 1.1*v.y, 1.1*v.z);
				glEnd();
			}
		}
	};

private:
};

#endif