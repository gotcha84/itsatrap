#version 120

// simple.vert
varying vec4 position;
varying vec3 normal;

void main(void)
{

  // ’¸“_ˆÊ’u
  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;


  /*gl_FrontColor = vec4(1.0, 0.0, 0.0, 1.0);
  
  gl_Position = ftransform();*/

  position = gl_ModelViewMatrix * gl_Vertex;
  normal = normalize(gl_NormalMatrix * gl_Normal);
  
  
  gl_Position = ftransform();
}
