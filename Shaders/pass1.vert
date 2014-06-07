#version 120
//
// pass1.vert
//
 
varying vec4 ambient;                                                 // 環境光の反射光 A
varying vec4 diffuse;                                                 // 拡散反射光 D
varying vec4 specular;                                                // 鏡面反射光 S
 
void main(void)
{
  vec3 position = vec3(gl_ModelViewMatrix * gl_Vertex);
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  
  vec3 light = normalize(gl_LightSource[0].position.xyz - position);  // その点から光源に向かう光線ベクトル L
  vec3 view = -normalize(position);                                   // その点から視点に向かう視線ベクトル V
  vec3 halfway = normalize(light + view);                             // 中間ベクトル H
  
  float nl = dot(normal, light);
  float nh = pow(dot(normal, halfway), gl_FrontMaterial.shininess);
  
  // 環境光
  ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
  
  // 拡散反射光
  diffuse = gl_FrontMaterial.diffuse * vec4(vec3(nl), 1.0) * gl_LightSource[0].diffuse;
  
  // 鏡面反射光
  specular = gl_FrontMaterial.specular * vec4(vec3(nh), 1.0) * gl_LightSource[0].specular;
  
  gl_Position = ftransform();                                         // 頂点位置の座標変換
}