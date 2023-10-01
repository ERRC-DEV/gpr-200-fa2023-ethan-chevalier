#version 450
out vec4 fragColor;
in vec2 uv;

uniform vec3 iResolution;
uniform float iTime;
in vec2 fragCoord;
void main(){
	/*FragColor = vec4(UV,0.0,1.0);*/
	// Normalized pixel coordinates (from 0 to 1)
    //vec2 division = vec2(iResolution.x,iResolution.y);
    vec2 uv = fragCoord;
    //division;

    // Time varying pixel color
    vec3 col = 0.0 + 0.5*sin(iTime+uv.xyx+vec3(0,2,4)+4);

    // Sun
    vec2 sunUV = uv * 2.0 -1.0;
    sunUV.x *= iResolution.x / iResolution.y;
    float d = distance(sunUV,vec2(0.0,sin(iTime)));
    d = smoothstep(0.5,0.6,d);
    col = mix(vec3(1.0,1.0,0.0),col,d);
    
    //Foreground
    float y = sin(((uv.x*4.0)+1.25)/2)*0.5+0.05;
    float t = step(y,uv.y);
    col = mix(vec3(0.0,0.55,0.0),col,t);
    
    /*
    //Foreground
    float y = sin((uv.x*4.0))*0.5+0.05;
    float t = step(y,uv.y);
    col = mix(vec3(0.0,0.55,0.0),col,t);
    */

    // Output to screen
    fragColor = vec4(col,1.0);
}
