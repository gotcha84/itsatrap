#pragma once
#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <math.h>

#include "tiny_obj_loader.h"
#include "Geode.h"
//#include "Utilities.h"
#include "Physics.h"

using namespace std;

class ObjModel : public sg::Geode {
	public:
		float m_cityScale;
		float m_canScale;
		float m_defaultScale;
		float m_turtleScale;
		glm::vec3 m_scaleVec;

		int m_id;
		string m_filename;

		//Physics m_physics;

		vector<vector<float>> m_vertices;
		vector<vector<float>> m_normals;
		vector<vector<int>> m_indices;
		vector<vector<float>> m_texcoords;
		vector<int> m_nIndices;
		vector<int> m_nVertices;
			
		ObjModel();
		ObjModel(int id);
		ObjModel(std::string filename);
		ObjModel(int id, std::string filename);
		~ObjModel();

		void initScales();

		void setVertices(vector<float> arr);
		void setNormals(vector<float> arr);
		void setTexcoords(vector<float> arr);
		void setIndices(vector<int> arr);
		void setNVertices(int ele);
		void setNIndices(int ele);
			
		void draw(glm::mat4 parent, glm::mat4 cam);
		void drawModel();

		void loadFilename(std::string filename);
		void loadModel(std::string filename);
		void loadModel();

		void print();
			
		void calculateBoundingBox();
		bool isInside(glm::vec3 point);
		bool collidesWith(ObjModel* o);
		void setMaterial();
};

#endif