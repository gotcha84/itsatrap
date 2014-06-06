#version 120
varying vec4 position;
varying vec3 normal; 
uniform sampler2D image;  
 

 void main(){
   vec4 color = texture2DProj(image, gl_TexCoord[0]);
   float intensity;
   vec2 pixSize = vec2(1.0/1600.0,1.0/1200.0);
   vec2 pos = gl_FragCoord.xy*pixSize;
   intensity = dot(texture2D(image,pos).xyz,vec3(0.3333,0.3333,0.3333));
   
   if(intensity > 0.99)
     gl_FragColor = vec4(1.0,1.0,1.0,1.0);
   else
	 gl_FragColor = vec4(0.0,0.0,0.0,1.0);
 }