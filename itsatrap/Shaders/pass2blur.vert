#version 120
//
// pass2.vert
//
 
varying vec2 texcoord;
varying vec4 position;
varying vec3 normal;
 
void main(void)
{
  position = gl_ModelViewMatrix * gl_Vertex;
normal = normalize(gl_NormalMatrix * gl_Normal);

//added for texture
gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

gl_Position = ftransform();
}