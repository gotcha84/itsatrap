#version 120
 
//1st tutorial
uniform vec3 color;
varying vec3 color2;
attribute vec3 art;
varying vec2 texcoord;

//2ndtutorial
varying vec3 position;
varying vec3 normal;

void main(void)
{
  //1st tutorial
  gl_Position= gl_ModelViewProjectionMatrix*gl_Vertex;
  color2 = art;
  texcoord = gl_MultiTexCoord0.xy;

  //2ndtutorial
  gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
  gl_Position= gl_ModelViewProjectionMatrix*gl_Vertex;
  position = vec3(gl_ModelViewProjectionMatrix*gl_Vertex);
  normal = gl_NormalMatrix*gl_Normal;
}
