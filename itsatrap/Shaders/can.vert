
#version 120

// simple.vert
varying vec4 position;
varying vec3 normal;

void main(void)
{

// Vertex location
//gl_FrontColor = vec4(1.0, 0.0, 0.0, 1.0);
position = gl_ModelViewMatrix * gl_Vertex;
normal = normalize(gl_NormalMatrix * gl_Normal);

//added for texture
gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

gl_Position = ftransform();
}

