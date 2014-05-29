
#version 150

// simple.frag
//added for texture
uniform sampler2D texture;

varying vec4 position;
varying vec3 normal;

void main (void)
{
// Fragment Color
//added for texture
vec4 color = texture2DProj(texture, gl_TexCoord[0]);

vec3 light = normalize((gl_LightSource[0].position * position.w - gl_LightSource[0].position.w * position).xyz);
float diffuse = max(dot(light, normal), 0.0);

vec3 view = -normalize(position.xyz);
vec3 halfway = normalize(light + view);
float specular = pow(max(dot(normal, halfway), 0.0), gl_FrontMaterial.shininess);

/*changed for texture*/
/*gl_FragColor = color * gl_LightSource[0].diffuse * diffuse
+ gl_FrontLightProduct[0].specular * specular
+ color * gl_LightSource[0].ambient;*/

gl_FragColor = color * (gl_LightSource[0].diffuse * diffuse + gl_LightSource[0].ambient)
+ gl_FrontLightProduct[0].specular * specular;
}
