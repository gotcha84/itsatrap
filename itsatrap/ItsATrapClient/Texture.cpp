#define NUM_TEX 100
#include "Texture.h"

Texture::Texture(){
	m_numTex = NUM_TEX;
	m_texID = new GLuint[m_numTex];

	for (int i = 0; i < m_numTex; i++) {
		m_texID[i] = 0;
	}

	glGenTextures(m_numTex, m_texID);
	if (glGetError()) cout << "[Texture.cpp] ERROR : could not glGenTextures in ctor!";

	initTextures();
}

Texture::~Texture() {
	delete[] m_texID;
	m_texID = nullptr;
}

// After adding enum, add texture file here
void Texture::initTextures() {
	loadTexture(m_texID[Textures::ActiveNode], "../Textures/resourceTower_tex_active.ppm");
	loadTexture(m_texID[Textures::InactiveNode], "../Textures/resourceTower_tex_Inactive.ppm");
	loadTexture(m_texID[Textures::BlueNode], "../Textures/resourceTower_tex_blue.ppm");
	loadTexture(m_texID[Textures::RedNode], "../Textures/resourceTower_tex_red.ppm");
	loadTexture(m_texID[Textures::Skybox], "../Textures/skybox.ppm");
	loadTexture(m_texID[Textures::RedPolynoid], "../Textures/Avatar/Polynoid_Red.ppm");
	loadTexture(m_texID[Textures::BluePolynoid], "../Textures/Avatar/Polynoid_Blue.ppm");
	loadTexture(m_texID[Textures::Caltrop], "../Textures/Obelisk3/ppm/Caltrop.ppm");
	loadTexture(m_texID[Textures::BRail], "../Textures/BRail.ppm");

	loadTexture(m_texID[Textures::Stunned], "../Textures/Stunned.ppm");
	loadTexture(m_texID[Textures::Slowed], "../Textures/Slowed.ppm");
	loadTexture(m_texID[Textures::Flashed], "../Textures/Flashed.ppm");
	
	loadTexture(m_texID[Textures::Base_01], "../Textures/Obelisk3/ppm/Base_01.ppm");
	loadTexture(m_texID[Textures::Base_02], "../Textures/Obelisk3/ppm/Base_02.ppm");
	loadTexture(m_texID[Textures::Base_03], "../Textures/Obelisk3/ppm/Base_03.ppm");
	loadTexture(m_texID[Textures::Base_04], "../Textures/Obelisk3/ppm/Base_04.ppm");
	loadTexture(m_texID[Textures::Base_05], "../Textures/Obelisk3/ppm/Base_05.ppm");
	loadTexture(m_texID[Textures::Base_06], "../Textures/Obelisk3/ppm/Base_06.ppm");
	loadTexture(m_texID[Textures::Base_07], "../Textures/Obelisk3/ppm/Base_07.ppm");
	loadTexture(m_texID[Textures::Base_08], "../Textures/Obelisk3/ppm/Base_08.ppm");
	loadTexture(m_texID[Textures::Base_09], "../Textures/Obelisk3/ppm/Base_09.ppm");
	loadTexture(m_texID[Textures::Base_10], "../Textures/Obelisk3/ppm/Base_10.ppm");

	loadTexture(m_texID[Textures::Base_11], "../Textures/Obelisk3/ppm/Base_11.ppm");
	loadTexture(m_texID[Textures::Base_12], "../Textures/Obelisk3/ppm/Base_12.ppm");
	loadTexture(m_texID[Textures::Base_13], "../Textures/Obelisk3/ppm/Base_13.ppm");
	loadTexture(m_texID[Textures::Base_14], "../Textures/Obelisk3/ppm/Base_14.ppm");
	loadTexture(m_texID[Textures::Base_15], "../Textures/Obelisk3/ppm/Base_15.ppm");
	loadTexture(m_texID[Textures::Base_16], "../Textures/Obelisk3/ppm/Base_16.ppm");
	loadTexture(m_texID[Textures::Base_17], "../Textures/Obelisk3/ppm/Base_17.ppm");
	loadTexture(m_texID[Textures::Base_18], "../Textures/Obelisk3/ppm/Base_18.ppm");
	loadTexture(m_texID[Textures::Base_19], "../Textures/Obelisk3/ppm/Base_19.ppm");
	loadTexture(m_texID[Textures::Base_20], "../Textures/Obelisk3/ppm/Base_20.ppm");

	loadTexture(m_texID[Textures::Base_21], "../Textures/Obelisk3/ppm/Base_21.ppm");
	loadTexture(m_texID[Textures::Base_22], "../Textures/Obelisk3/ppm/Base_22.ppm");
	loadTexture(m_texID[Textures::Base_23], "../Textures/Obelisk3/ppm/Base_23.ppm");
	loadTexture(m_texID[Textures::Base_24], "../Textures/Obelisk3/ppm/Base_24.ppm");
	loadTexture(m_texID[Textures::Base_25], "../Textures/Obelisk3/ppm/Base_25.ppm");
	loadTexture(m_texID[Textures::Base_26], "../Textures/Obelisk3/ppm/Base_26.ppm");
	loadTexture(m_texID[Textures::Base_27], "../Textures/Obelisk3/ppm/Base_27.ppm");

	loadTexture(m_texID[Textures::Bridge], "../Textures/Obelisk3/ppm/Bridge_01.ppm");
	loadTexture(m_texID[Textures::Float], "../Textures/Obelisk3/ppm/Float_01.ppm");

	loadTexture(m_texID[Textures::Ext_01], "../Textures/Obelisk3/ppm/Ext_01.ppm");
	loadTexture(m_texID[Textures::Ext_02], "../Textures/Obelisk3/ppm/Ext_02.ppm");
	loadTexture(m_texID[Textures::Ext_03], "../Textures/Obelisk3/ppm/Ext_03.ppm");
	loadTexture(m_texID[Textures::Ext_04], "../Textures/Obelisk3/ppm/Ext_04.ppm");
	loadTexture(m_texID[Textures::Ext_05], "../Textures/Obelisk3/ppm/Ext_05.ppm");

	loadTexture(m_texID[Textures::Wedge_01], "../Textures/Obelisk3/ppm/Wedge_01.ppm");
	loadTexture(m_texID[Textures::Rail_01], "../Textures/Obelisk3/ppm/Rail_01.ppm");
	loadTexture(m_texID[Textures::Metal_01], "../Textures/Obelisk3/ppm/Metal_01.ppm");
	loadTexture(m_texID[Textures::ERail_01], "../Textures/Obelisk3/ppm/ERail_01.ppm");
	loadTexture(m_texID[Textures::Climbable_01], "../Textures/Obelisk3/ppm/Climbable_01.ppm");
}

void Texture::loadTexture(GLuint id, const char* the_texture) {
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	//glUniform1i(glGetUniformLocation(gl2Program, "texture"), 0);

	// Load image file
	tdata = loadPPM(the_texture, twidth, theight);
	if (tdata == NULL) cout << "[Texture.cpp] Error loading PPM : " << the_texture << endl;

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, id);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}


unsigned char* Texture::loadPPM(const char* filename, int& width, int& height) {
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL) {
		std::cout << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do {
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do {
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1) {
		std::cout << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}