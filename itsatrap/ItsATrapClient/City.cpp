#include "City.h"

namespace sg {

	City::City() {
		initArrays();
	}

	/*City::City(string filename) {
		World::m_heightMapXShift = 278;
		World::m_heightMapZShift = 463;
		initArrays();
		loadData(filename);
	}*/

	City::~City() {
		for (int i = 0; i < 100; i++) {
			delete[] m_vertices[i];
			m_vertices[i] = nullptr;

			delete[] m_normals[i];
			m_normals[i] = nullptr;

			//delete[] m_texcoords[i];
			//m_texcoords = nullptr;

			delete[] m_indices[i];
			m_indices[i] = nullptr;
		}

		delete[] m_vertices;
		m_vertices = nullptr;
		delete[] m_nVertices;
		m_nVertices = nullptr;
		delete[] m_normals;
		m_normals = nullptr;
		//delete[] m_texcoords;
		//m_texcoords = nullptr;
		delete[] m_indices;
		m_indices = nullptr;
		delete[] m_nIndices;
		m_nIndices = nullptr;

		while (hasChild()) {
			delete m_child[0];
			m_child[0] = nullptr;
			m_child.erase(m_child.begin());
		}
	}

	void City::initArrays() {
		m_vertices = new float*[100];
		m_nVertices = new int[100];
		m_normals = new float*[100];
		//m_texcoords = new float*[100];
		m_indices = new int*[100];
		m_nIndices = new int[100];

		for (int i = 0; i < 100; i++) {
			m_vertices[i] = new float[1000];
			m_normals[i] = new float[1000];
			//m_texcoords[i] = new float[1000];
			m_indices[i] = new int[1000];
		}
	}

	void City::draw(glm::mat4 parent, glm::mat4 camera) {

		for (int i = 0; i < getNumChildren(); i++) {
			m_child[i]->draw(parent, camera);
		}
		
		//glm::mat4 cam_inverse = glm::inverse(camera);
		//glm::mat4 mv = cam_inverse * parent;
		////cout << "nverts is : " << m_nIndices[0] << endl;
		//glPushMatrix();
		//	glMatrixMode(GL_MODELVIEW);
		//	glLoadMatrixf(glm::value_ptr(mv));

		//	glColor3f(color.x, color.y, color.z);
		//	drawShape();
		//glPopMatrix();
	}

	
	void City::updateHeightMap() {

		float minx;
		float maxx;
		float minz;
		float maxz;
		float miny;
		float maxy;
		float tmpminx = FLT_MAX;
		float tmpminy = FLT_MAX;
		float tmpminz = FLT_MAX;
		float tmpmaxx = FLT_MIN;
		float tmpmaxy = FLT_MIN;
		float tmpmaxz = FLT_MIN;

		//int arr[1][2] = {{0, 1}};
		int arr[4][2] = {{0, 1}, {0, 3}, {1, 3}, {2, 3}};  
		//int arr[5][2] = {{0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}};   
		//if (!init_heightMap) {
		for (int k = 0; k < 46; k++) {
			for (int i = 0; i < m_nVertices[k]; i++) {
				m_vertices[k][i]/=10.0f;
				/*
				if (i % 3 == 1) {
					m_vertices[k][i]+=4.0f;
				}
				*/
			}
		}
		//}

		// TODO: fix min/max (only takes 2 args)
		for (int l = 0; l < 46; l++) {
			//for (int i = 0; i < m_nVertices[l]-m_nVertices[l]%9; i+=9) { 
			for (int i = 0; i < m_nVertices[l]-m_nVertices[l]%12; i+=12) { 

				tmpminx = min(min(m_vertices[l][i], m_vertices[l][i+3]),  min(m_vertices[l][i+6], m_vertices[l][i+9]));
				tmpmaxx = max(max(m_vertices[l][i], m_vertices[l][i+3]),  max(m_vertices[l][i+6], m_vertices[l][i+9]));
				tmpminy = min(min(m_vertices[l][i+1], m_vertices[l][i+4]),  min(m_vertices[l][i+7], m_vertices[l][i+10]));
				tmpmaxy = max(max(m_vertices[l][i+1], m_vertices[l][i+4]),  max(m_vertices[l][i+7], m_vertices[l][i+10]));
				tmpminz = min(min(m_vertices[l][i+2], m_vertices[l][i+5]),  min(m_vertices[l][i+8], m_vertices[l][i+11]));
				tmpmaxz = max(max(m_vertices[l][i+2], m_vertices[l][i+5]),  max(m_vertices[l][i+8], m_vertices[l][i+11]));


				if (tmpminx < 0) {
					minx = -1*floor((-1.0f*tmpminx)+0.5f);
				}
				else {
					minx = floor(tmpminx+0.5f);
				}

				if (tmpminz < 0) {
					minz = -1*floor((-1.0f*tmpminz)+0.5f);
				}
				else {
					minz = floor(tmpminz+0.5f);
				}

				if (tmpmaxx < 0) {
					maxx = -1*floor((-1.0f*tmpmaxx)+0.5f);
				}
				else {
					maxx = floor(tmpmaxx+0.5f);
				}

				if (tmpmaxz < 0) {
					maxz = -1*floor((-1.0f*tmpmaxz)+0.5f);
				}
				else {
					maxz = floor(tmpmaxz+0.5f);
				}

				if (tmpminy < 0) {
					miny = -1*floor((-1.0f*tmpminy)+0.5f);
				}
				else {
					miny = floor(tmpminy+0.5f);
				}

				if (tmpmaxy < 0) {
					maxy = -1*floor((-1.0f*tmpmaxy)+0.5f);
				}
				else {
					maxy = floor(tmpmaxy+0.5f);
				}

				for (int j = minx; j < maxx; j++) {
					for (int k = minz; k < maxz; k++) {
						if (maxy > World::m_heightMap[j+World::m_heightMapXShift][k+World::m_heightMapZShift]) {
							World::m_heightMap[j+World::m_heightMapXShift][k+World::m_heightMapZShift] = maxy;
						}
					}
				}
			}
		}
	
		// player is 4 feet "tall"
		/*for (int i = 0; i < 1019; i++) {
			for (int j = 0; j < 787; j++) {
					World::m_heightMap[i][j]+=4;
					//cout << World::m_heightMap[i][j] << endl;
			}
		}*/

	
	}
	

	// TODO: implement initializeHeightMap
	/*
	void City::loadData() {
		std::string inputfile = "city.obj";
		std::vector<tinyobj::shape_t> shapes;
  
		std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  
		int indicesCount = 0;
		int verticesCount = 0;
		int texturesCount = 0;
		int normalsCount = 0;
		int max_ele = -1;

		int tmpIndicesCount = 0;
		int tmpVerticesCount = 0;
		int tmpTexturesCount = 0;
		int tmpNormalsCount = 0;
		//cout << shapes.size() << endl;
		for (int j = 0; j < shapes.size(); j++) {
			tmpIndicesCount = 0;
			tmpVerticesCount = 0;
			tmpTexturesCount = 0;
			tmpNormalsCount = 0;
			for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
				m_Indices[i+indicesCount] = shapes[j].mesh.indices[i]+indicesCount;
				//cout << m_Indices[i+indicesCount] << endl;
				tmpIndicesCount++;
			}
			cout << tmpIndicesCount << endl;
			indicesCount += tmpIndicesCount;
		
			for (int i = 0; i < shapes[j].mesh.positions.size(); i++) {
				m_vertices[i+verticesCount] = shapes[j].mesh.positions[i];
				tmpVerticesCount++;
			}
		
			verticesCount += tmpVerticesCount;

			for (int i = 0; i < shapes[j].mesh.normals.size(); i++) {
				m_normals[i+normalsCount] = shapes[j].mesh.normals[i];
				tmpNormalsCount++;
			}
			normalsCount += tmpNormalsCount;

			for (int i = 0; i < shapes[j].mesh.texcoords.size(); i++) {
				m_Texcoords[i+texturesCount] = shapes[j].mesh.texcoords[i];
				tmpTexturesCount++;
			}
			texturesCount +=tmpTexturesCount;
	
		}
		m_nIndices = indicesCount;
		m_nVertices = verticesCount;

		Utilities::writeIntArrayToFile(m_Indices, m_nIndices, "new_indices.txt");
		Utilities::writeFloatArrayToFile(m_vertices, m_nVertices, "new_vertices.txt");
		Utilities::writeFloatArrayToFile(m_normals, normalsCount, "new_normals.txt");
		Utilities::writeFloatArrayToFile(m_Texcoords, texturesCount, "new_textures.txt");

	}
	*/

	void City::loadData(string inputfile) {
		std::vector<tinyobj::shape_t> shapes;
  
		std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  
		int indicesCount = 0;
		int verticesCount = 0;
		int texturesCount = 0;
		int normalsCount = 0;
		int max_ele = -1;

		int tmpIndicesCount = 0;
		int tmpVerticesCount = 0;
		int tmpTexturesCount = 0;
		int tmpNormalsCount = 0;
		//cout << shapes.size() << endl;
		for (int j = 0; j < 46; j++) {
			tmpIndicesCount = 0;
			tmpVerticesCount = 0;
			tmpTexturesCount = 0;
			tmpNormalsCount = 0;
			for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
				m_indices[j][i] = shapes[j].mesh.indices[i];
				//cout << m_Indices[i+indicesCount] << endl;
			
				tmpIndicesCount++;
			}

			indicesCount += tmpIndicesCount;
		
			for (int i = 0; i < shapes[j].mesh.positions.size(); i++) {
				m_vertices[j][i] = shapes[j].mesh.positions[i];
				tmpVerticesCount++;
			}
		
			verticesCount += tmpVerticesCount;

			for (int i = 0; i < shapes[j].mesh.normals.size(); i++) {
				m_normals[j][i] = shapes[j].mesh.normals[i];
				tmpNormalsCount++;
			}
			normalsCount += tmpNormalsCount;

			// TODO textures
			/*for (int i = 0; i < shapes[j].mesh.texcoords.size(); i++) {
				m_Texcoords[j][i+texturesCount] = shapes[j].mesh.texcoords[i];
				tmpTexturesCount++;
			}
			texturesCount +=tmpTexturesCount;*/
			m_nIndices[j] = tmpIndicesCount;
			m_nVertices[j] = tmpVerticesCount;

			Building *newChild = new Building(j);
			addChild(newChild);

		

			//delete newChild;
			// TODO delete
		}

		Utilities::writeIntArrayToFile(m_nVertices, 100, "nverts.txt");
		Utilities::writeFloatArrayToFile(m_vertices[0], 1000, "verts0.txt");
		
		updateHeightMap();

		for (int i = 0; i < 46; i++) {
			((sg::Building*)m_child[i])->calculateBoundingBox();
		}

	}

	//void Window::drawShape(int nVerts, vector<float> vertices, vector<float> normals, vector<float> texcoords) {



}