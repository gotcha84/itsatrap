#version 120

//1st tutorial
uniform sampler2D img;
uniform vec3 color;
varying vec3 color2;
varying vec2 texcoord;


//2ndtutorial
//uniform sampler2D img;
varying vec3 position;
varying vec3 normal;
uniform vec3 lightPos;
uniform vec3 mambient; //gl_FrontMaterial
uniform vec3 mdiffuse;
uniform vec3 mspecular;
uniform float shininess;
uniform vec3 lambient;  //gl_Lightsource[0]
uniform vec3 ldiffuse;
uniform vec3 lspecular;


 
void main (void)
{
  //1st tutorial
  vec3 texcolor =vec3( texture2D(img,texcoord));
  
  //2ndtutorial
  float dist = length(position-lightPos);
  float att=1.0/(1.0+0.01*dist+0.01*dist*dist);
  vec4 color = texture2DProj(img, gl_TexCoord[0]);
  vec3 ambient = texcolor*lambient;
  vec3 surf2light = normalize((lightPos-position).xyz);
  vec3 norm = normalize(normal.xyz);
  float dcont = max(0.0,dot(norm,surf2light));
  vec3 diffuse = dcont*texcolor*ldiffuse;
  vec3 surf2view = normalize(-position.xyz);
  vec3 reflection = reflect(-surf2light,norm);
  float scont = pow(max(0.0,dot(surf2view,reflection)),shininess);
  vec3 specular = scont*lspecular*mspecular;
  
  //lits at all
  gl_FragColor = vec4(ambient+diffuse+specular,1.0);
  //not lights from far 
  //gl_FragColor = vec4(ambient+diffuse+specular*att,1.0);
  //lights around me
  //gl_FragColor = vec4((ambient+diffuse+specular)*att,1.0);
}
