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
#include "Utilities.h"

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

		bool m_flipTex;
		GLuint m_texID;

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
		
		void setTexture(GLuint tex);
		void flipTexture();
		void unflipTexture();

		void print();
		
		void calculateBoundingBox();
		void calculateBoundingBox(glm::mat4 model);
		bool isInside(glm::vec3 point);
		bool collidesWith(ObjModel* o);
		void setMaterial();
		void setMaterial(float ambient[4], float diffuse[4], float specular[4], float emission[4], float shininess);
		void addMaterial(GLfloat ambient[4], GLfloat diffuse[4], GLfloat specular[4], GLfloat emission[4], GLfloat shininess);
};

#endif