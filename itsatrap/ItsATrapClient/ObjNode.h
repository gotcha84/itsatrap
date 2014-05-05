#pragma once
#ifndef OBJNODE_H
#define OBJNODE_H

#include <vector>
#include <algorithm>
#include <climits>
#include <math.h>

#include "tiny_obj_loader.h"
#include "Utilities.h"
#include "Geode.h"
#include "Physics.h"

using namespace std;

namespace sg {
	class ObjNode : public Geode {
		public:

			Physics m_physics;

			float m_cityScale;
			float m_canScale;
			float m_defaultScale;
			float m_turtleScale;
			glm::vec3 m_scaleVec;

			int m_id;
			string m_objFilename;

			vector<vector<float>> m_vertices;
			vector<vector<float>> m_normals;
			vector<vector<int>> m_indices;
			vector<vector<float>> m_texcoords;
			vector<int> m_nIndices;
			vector<int> m_nVertices;
			
			ObjNode();
			ObjNode(int id);
			~ObjNode();

			void setVertices(vector<float> arr);
			void setNormals(vector<float> arr);
			void setTexcoords(vector<float> arr);
			void setIndices(vector<int> arr);
			void setNVertices(int ele);
			void setNIndices(int ele);
			
			virtual void draw(glm::mat4 parent, glm::mat4 cam);
			virtual void drawShape();

			virtual void loadData();

			void print();
			
			virtual void calculateBoundingBox();
			virtual bool isInside(glm::vec3 point);
			virtual bool collidesWith(ObjNode* o); // make virtual if child classes
			virtual void setMaterial();
	};
}

#endif