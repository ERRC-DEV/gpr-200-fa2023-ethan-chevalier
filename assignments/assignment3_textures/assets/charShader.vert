#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
uniform float _time;
out vec2 UV;
void main(){
	UV = vUV;
	float displacement = sin(_time);
	gl_Position = vec4(((vPos.x+displacement)/2),(vPos.y/2),(vPos.z/2),1.0);
}