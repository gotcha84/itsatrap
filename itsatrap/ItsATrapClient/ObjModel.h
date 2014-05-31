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
#include "Texture.h"
#include "SOIL.h"

using namespace std;

class ObjModel : public sg::Geode {
	public:
		float m_cityScale;
		float m_canScale;
		float m_defaultScale;
		float m_turtleScale;
		glm::vec3 m_scaleVec;

		glm::vec3 m_position;
		int m_id;
		string m_objFilename;
		string m_mtlFilename;
		
		Texture *m_texture;
		GLuint m_textureID;

		vector<vector<float>> m_vertices;
		vector<vector<float>> m_normals;
		vector<vector<int>> m_indices;
		vector<vector<float>> m_texcoords;
		vector<int> m_nIndices;
		vector<int> m_nVertices;
			
		ObjModel();
		ObjModel(int id);
		ObjModel(string objFilename);
		ObjModel(string objFilename, string mtlFilename);
		ObjModel(string objFilename, glm::vec3 currPos);
		ObjModel(string objFilename, string mtlFilename, glm::vec3 currPos);
		ObjModel(int id, string objFilename);
		ObjModel(int id, string objFilename, string mtlFilename);
		~ObjModel();

		void initCommon();

		void setVertices(vector<float> arr);
		void setNormals(vector<float> arr);
		void setTexcoords(vector<float> arr);
		void setIndices(vector<int> arr);
		void setNVertices(int ele);
		void setNIndices(int ele);
			
		void draw(glm::mat4 parent, glm::mat4 cam);
		void drawModel();

		void loadFilename(string objFilename);
		void loadFilename(string objFilename, string mtlFilename);
		void loadModel(string objFilename);
		void loadModel(string objFilename, string mtlFilename);
		void loadModel();

		void loadTexture(string filename);

		void print();
			
		void calculateBoundingBox();
		bool isInside(glm::vec3 point);
		bool collidesWith(ObjModel* o);
		void setMaterial();
};

#endif