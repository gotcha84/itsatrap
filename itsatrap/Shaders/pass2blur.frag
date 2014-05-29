#version 120
uniform sampler2D image;  
varying vec4 position;
varying vec3 normal; 

/* uniform float offset[5] = float[]( 0.0, 1.0, 2.0, 3.0, 4.0 );  
 uniform float weight[5] = float[]( 0.2270270270, 0.1945945946, 0.1216216216,  
                                    0.0540540541, 0.0162162162 );  
                                    
 void main(void)  

 {  
	for(int j = 0; j < 10; j++){
     gl_FragColor = texture2D( image, vec2(gl_FragCoord)/512.0 ) * weight[0];  
     for (int i=1; i<5; i++) {  
         gl_FragColor +=  
             texture2D( image, ( vec2(gl_FragCoord)+vec2(0.0, offset[i]) )/512.0 )  
                 * weight[i];  
         gl_FragColor +=  
             texture2D( image, ( vec2(gl_FragCoord)-vec2(0.0, offset[i]) )/512.0 )  
                 * weight[i];  
     }  
	 }

 } */

 /*void main(){
   vec4 color = texture2DProj(image, gl_TexCoord[0]);
   float intensity;
   vec2 pixSize = vec2(1.0/512.0,1.0/512.0);
   vec2 pos = gl_FragCoord.xy*pixSize;
   intensity = dot(texture2D(image,pos).xyz,vec3(0.3333,0.3333,0.3333));
   
   if(intensity > 0.99)
     gl_FragColor = vec4(1.0,1.0,1.0,1.0);
   else
	 gl_FragColor = vec4(0.0,0.0,0.0,1.0);
 }*/

 uniform sampler2D bgl_RenderedTexture;

void main()
{
   vec4 sum = vec4(0);
   vec2 texcoord = vec2(gl_TexCoord[0]);
   int j;
   int i;
   vec4 color = texture2DProj(image, gl_TexCoord[0]);
   for( i= -4 ;i < 4; i++)
   {
        for (j = -3; j < 3; j++)
        {
            sum += texture2D(bgl_RenderedTexture, texcoord + vec2(j, i)*0.004) * 0.25;
        }
   }
       if (texture2D(bgl_RenderedTexture, texcoord).r < 0.3)
    {
       gl_FragColor = sum*sum*0.09 + texture2D(bgl_RenderedTexture, texcoord);
    }
    else
    {
        if (texture2D(bgl_RenderedTexture, texcoord).r < 0.7)
        {
            gl_FragColor = sum*sum*0.059 + texture2D(bgl_RenderedTexture, texcoord);
        }
        else
        {
            gl_FragColor = sum*sum*0.0275 + texture2D(bgl_RenderedTexture, texcoord);
        }
    }
	gl_FragColor = color;
}