#include "City.h"
#include "Client.h"

namespace sg {
	
	City::City() {
		m_numBuildings = 0;
		m_maxNumArrays = 1000;
		m_maxArraySize = 10000;
		m_cityScale = 0.1f;
		m_canScale = 5.0f;
		m_defaultScale = 1.0f;
		initArrays();
	}

	/*City::City(string filename) {
		World::m_heightMapXShift = 278;
		World::m_heightMapZShift = 463;
		initArrays();
		loadData(filename);
	}*/

	City::~City() {
		for (int i = 0; i < m_maxNumArrays; i++) {
			delete[] m_vertices[i];
			m_vertices[i] = nullptr;

			delete[] m_normals[i];
			m_normals[i] = nullptr;

			delete[] m_texcoords[i];
			m_texcoords = nullptr;

			delete[] m_indices[i];
			m_indices[i] = nullptr;
		}

		delete[] m_vertices;
		m_vertices = nullptr;
		delete[] m_nVertices;
		m_nVertices = nullptr;
		delete[] m_normals;
		m_normals = nullptr;
		delete[] m_texcoords;
		m_texcoords = nullptr;
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
		m_vertices = new float*[m_maxNumArrays];
		m_nVertices = new int[m_maxNumArrays];
		m_normals = new float*[m_maxNumArrays];
		m_texcoords = new float*[m_maxNumArrays];
		m_indices = new int*[m_maxNumArrays];
		m_nIndices = new int[m_maxNumArrays];

		for (int i = 0; i < m_maxNumArrays; i++) {
			m_vertices[i] = new float[m_maxArraySize];
			m_normals[i] = new float[m_maxArraySize];
			m_texcoords[i] = new float[m_maxArraySize];
			m_indices[i] = new int[m_maxArraySize];
		}
	}

	void City::draw(glm::mat4 parent, glm::mat4 camera) {
		int count = 0;

		for (int i = 0; i < getNumChildren(); i++) {
			//cout << "city count: " << count << endl;
			//if (i == 2) {
			count++;
			//cout << "in loop: " << m_child[i]. << endl;
			m_child[i]->draw(parent, camera);
			//((sg::Building*)m_child[i])->draw(parent, camera);
			//}
		}

		
		//glm::mat4 cam_inverse = glm::inverse(camera);
		//glm::mat4 mv = cam_inverse * parent;
		////cout << "nverts is : " << m_nIndices[0] << endl;
		//glPushMatrix();
		//	glMatrixMode(GL_MODELVIEW);
		//	glLoadMatrixf(glm::value_ptr(mv));

		//	glColor4f(color.r, color.g, color.b, color.a);
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
		float tmpmaxx = -1.0f*FLT_MAX;
		float tmpmaxy = -1.0f*FLT_MAX;
		float tmpmaxz = -1.0f*FLT_MAX;

		int arr[4][2] = {{0, 1}, {0, 3}, {1, 3}, {2, 3}};  

		for (int l = 0; l < m_numBuildings; l++) {
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
	/* UNUSED
	bool City::loadDataAtPlace(string inputfile, glm::vec3 position) {

		vector<tinyobj::shape_t> shapes;
  
		string err = tinyobj::LoadObj(shapes, inputfile.c_str());

		int indicesCount = 0;
		int verticesCount = 0;
		int texturesCount = 0;
		int normalsCount = 0;
		int max_ele = -1;

		int tmpIndicesCount = 0;
		int tmpVerticesCount = 0;
		int tmpTexturesCount = 0;
		int tmpNormalsCount = 0;

		int num_children = getNumChildren();

		int added = 0;
		std::string tmp;
		stringstream ss;
		glm::vec3 avg = Utilities::findAverage(inputfile);

		float xTranslation;
		float yTranslation;
		float zTranslation;

		for (int j = 0; j < shapes.size(); j++) {
			tmpIndicesCount = 0;
			tmpVerticesCount = 0;
			tmpTexturesCount = 0;
			tmpNormalsCount = 0;
			for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
				m_indices[j+num_children][i] = shapes[j].mesh.indices[i];
				//cout << m_indices[i+indicesCount] << endl;
				tmpIndicesCount++;
			}
			
			indicesCount += tmpIndicesCount;
		
			vector<float> tmpArr;

			if (inputfile == "city.obj") {
				xTranslation = position.x - (m_cityScale*avg.x);
				yTranslation = position.y - (m_cityScale*avg.y);
				zTranslation = position.z - (m_cityScale*avg.z);
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_cityScale, xTranslation, yTranslation, zTranslation);
			}
			else if (inputfile == "Can.obj") {
				xTranslation = position.x - (m_canScale*avg.x);
				yTranslation = position.y - (m_canScale*avg.y);
				zTranslation = position.z - (m_canScale*avg.z);
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_canScale, xTranslation, yTranslation, zTranslation);
			}
			else {
				xTranslation = position.x - (m_defaultScale*avg.x);
				yTranslation = position.y - (m_defaultScale*avg.y);
				zTranslation = position.z - (m_defaultScale*avg.z);
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_defaultScale, xTranslation, yTranslation, zTranslation);
			}
			for (int i = 0; i < shapes[j].mesh.positions.size(); i++) {
				m_vertices[j+num_children][i] = tmpArr[i];
				tmpVerticesCount++;
			}
			verticesCount += tmpVerticesCount;

			for (int i = 0; i < shapes[j].mesh.normals.size(); i++) {
				m_normals[j+num_children][i] = shapes[j].mesh.normals[i];
				tmpNormalsCount++;
			}
			normalsCount += tmpNormalsCount;

			// TODO textures
			for (int i = 0; i < shapes[j].mesh.texcoords.size(); i++) {
				m_texcoords[j+num_children][i] = shapes[j].mesh.texcoords[i];
				tmpTexturesCount++;
			}
			texturesCount +=tmpTexturesCount;
			m_nIndices[j+num_children] = tmpIndicesCount;
			m_nVertices[j+num_children] = tmpVerticesCount;

			Building *newChild = new Building(j+num_children);
			addChild(newChild);
			m_numBuildings++;
			added++;



			// TODO delete newchild
		}

		//Utilities::writeIntArrayToFile(m_nVertices, added, "nverts.txt");
		
		updateHeightMap();

		for (int i = 0; i < added; i++) {
			((sg::Building*)m_child[i+num_children])->calculateBoundingBox();
			((sg::Building*)m_child[i+num_children])->setMaterial();
		}

		bool canPlace = true;
		for (int i = 0; i < added; i++) {
			// TODO: check if child is buildign?
			for (int j = 0; j < num_children; j++) {
				sg::Building* child = (sg::Building*)m_child[j];
				if (((sg::Building*)m_child[i+num_children])->collidesWith(child)) {
					canPlace = false;
					break;
				}
			}
		}
		if (!canPlace) {
			//cout << "BEFORE DELETION: " << num_children << endl;
			for (int i = num_children+added-1; i > num_children-1; i--) {
				//cout << "DELETING! " << i-num_children << " of " << added << endl;
				sg::Node* deleteMe = m_child[i];
			

				m_nIndices[i] = 0;
				m_nVertices[i] = 0;
				m_numBuildings--;
				bool result = removeChild(i);
				//cout << "result: " << result << endl;
				//delete deleteMe;
				//deleteMe = nullptr;

				//delete[] m_vertices[i];
				//m_vertices[i] = nullptr;

				//delete[] m_normals[i];
				//m_normals[i] = nullptr;

				//delete[] m_texcoords[i];
				//m_texcoords = nullptr;

				//delete[] m_indices[i];
				//m_indices[i] = nullptr;
			}
			//cout << "AFTER DELETION: " << getNumChildren() << endl;
		}
		return canPlace;
	}
	*/
	void City::loadData(string inputfile) {

		vector<tinyobj::shape_t> shapes;
  
		string err = tinyobj::LoadObj(shapes, inputfile.c_str());

		int indicesCount = 0;
		int verticesCount = 0;
		int texturesCount = 0;
		int normalsCount = 0;
		int max_ele = -1;

		int tmpIndicesCount = 0;
		int tmpVerticesCount = 0;
		int tmpTexturesCount = 0;
		int tmpNormalsCount = 0;

		int num_children = getNumChildren();

		int added = 0;
		std::string tmp;
		stringstream ss;
		
		for (int j = 0; j < shapes.size(); j++) {
			tmpIndicesCount = 0;
			tmpVerticesCount = 0;
			tmpTexturesCount = 0;
			tmpNormalsCount = 0;
			for (int i = 0; i < shapes[j].mesh.indices.size(); i++) {
				m_indices[j+num_children][i] = shapes[j].mesh.indices[i];
				//cout << m_indices[i+indicesCount] << endl;
				tmpIndicesCount++;
			}

			indicesCount += tmpIndicesCount;
		
			vector<float> tmpArr;

			if (inputfile == "city.obj") {
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_cityScale, 0.0f, -1.0f, 0.0f);
			}
			else if (inputfile == "Can.obj") {
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_canScale, 0.0f, 0.0f, 0.0f);
			}
			else {
				tmpArr = Utilities::modifyVec(shapes[j].mesh.positions, m_defaultScale, 0.0f, 0.0f, 0.0f);
			}

			for (int i = 0; i < shapes[j].mesh.positions.size(); i++) {
				m_vertices[j+num_children][i] = tmpArr[i];
				tmpVerticesCount++;
			}
			verticesCount += tmpVerticesCount;

			for (int i = 0; i < shapes[j].mesh.normals.size(); i++) {
				m_normals[j+num_children][i] = shapes[j].mesh.normals[i];
				tmpNormalsCount++;
			}
			normalsCount += tmpNormalsCount;

			// TODO textures
			for (int i = 0; i < shapes[j].mesh.texcoords.size(); i++) {
				m_texcoords[j+num_children][i] = shapes[j].mesh.texcoords[i];
				tmpTexturesCount++;
			}
			texturesCount +=tmpTexturesCount;
			m_nIndices[j+num_children] = tmpIndicesCount;
			m_nVertices[j+num_children] = tmpVerticesCount;

			Building *newChild = new Building(j+num_children);
			addChild(newChild);
			m_numBuildings++;
			added++;

			/*ss.clear();
			ss << j;
			tmp = ss.str();
			Utilities::writeFloatArrayToFile(m_vertices[j], shapes[j].mesh.indices.size(), "verts"+tmp+".txt");
			*/

			// TODO delete newchild
		}

		//Utilities::writeIntArrayToFile(m_nVertices, added, "nverts.txt");
		
		updateHeightMap();

		for (int i = 0; i < added; i++) {
			sg::Building *buildingPtr = ((sg::Building*)m_child[i+num_children]);
			buildingPtr->calculateBoundingBox();
			Client::sendStaticObject(buildingPtr->m_boundingBox.m_minX, buildingPtr->m_boundingBox.m_minY, buildingPtr->m_boundingBox.m_minZ, 
				buildingPtr->m_boundingBox.m_maxX, buildingPtr->m_boundingBox.m_maxY, buildingPtr->m_boundingBox.m_maxZ);
			buildingPtr->setMaterial();
		}



	}

	void City::print() {
		cout << "(" << this->getObjectID() << " City: " << this->getName() << ")";
	}

}