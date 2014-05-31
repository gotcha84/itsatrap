// simple.vert
 
void main(void)
{
  vec4 position = gl_ModelViewMatrix * gl_Vertex;
  vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
  vec3 light = normalize((gl_LightSource[0].position * position.w - gl_LightSource[0].position.w * position).xyz);
  float diffuse = max(dot(light, normal), 0.0);
  
  vec3 view = -normalize(position.xyz);
  vec3 halfway = normalize(light + view);
  float specular = pow(max(dot(normal, halfway), 0.0), gl_FrontMaterial.shininess);
  gl_FrontColor = gl_FrontLightProduct[0].diffuse * diffuse
                + gl_FrontLightProduct[0].specular * specular
                + gl_FrontLightProduct[0].ambient;
  
  gl_Position = ftransform();
}