#version 120
//
// pass1.frag
//
 
varying vec4 ambient;                                                 // 環境光の反射光 A
varying vec4 diffuse;                                                 // 拡散反射光 D
varying vec4 specular;                                                // 鏡面反射光 S
 
void main(void)
{
  /*gl_FragData[0] = diffuse;
  gl_FragData[1] = specular;
  gl_FragData[2] = ambient;*/
  //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}