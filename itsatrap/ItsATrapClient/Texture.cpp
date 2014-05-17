#include "Texture.h"


	
GLuint buildingTexture1;

Texture::Texture(){
	texture_building = "../Models/building1.ppm";
}
			
Texture::~Texture(){}

GLuint Texture::loadTexture(char* the_texture) {
	//GLuint texture;			 // storage for one texture
	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	//glUniform1i(glGetUniformLocation(gl2Program, "texture"), 0);

	// Load image file
	tdata = loadPPM(the_texture, twidth, theight);
	GLuint i = -1;
	if (tdata==NULL) return i;
	//int num;
	
	glGenTextures(1, &buildingTexture1);   
		
	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, buildingTexture1);
	
	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	
	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return buildingTexture1;

}


unsigned char* Texture::loadPPM(char* filename, int& width, int& height) {
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ( (fp=fopen(filename, "rb")) == NULL) {
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do {
		retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width  = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do {
		retval_fgets=fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1) {
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}
