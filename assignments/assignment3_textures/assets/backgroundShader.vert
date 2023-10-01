#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
uniform float _time;
out vec2 UV;
void main(){
	UV = vUV;
	float multiplier = abs(sin(_time)) + 1;

	gl_Position = vec4((vPos*multiplier),1.0);
}