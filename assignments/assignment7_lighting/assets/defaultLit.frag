#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal

}fs_in;

struct Light {
	vec3 position; //World space
	vec3 color; //RGB
};

struct Material {
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shininess; //Shininess
};

uniform sampler2D _Texture;
uniform Material _Properties;
uniform Light _Light;
uniform vec3 _Camerapos;

void main(){
	FragColor = texture(_Texture,fs_in.UV);
	vec3 normal = normalize(fs_in.WorldNormal);

	//Diffuse Lighting
	vec3 lightVector = normalize(_Light.position - fs_in.WorldPosition);
	float tempDiff = max(dot(normal, lightVector), 0.0);
	vec3 diffuse = _Properties.diffuseK * vec3(tempDiff, tempDiff, tempDiff);
	
	//Specular
	vec3 relfectVector = reflect((lightVector*-1),normal);
	vec3 cameraVector = _Camerapos - fs_in.WorldPosition;
	float tempSpecular = _Properties.specular * pow(max(dot(relfectVector, cameraVector), 0), _Properties.shininess);
	vec3 specular = vec3(tempSpecular, tempSpecular, tempSpecular);

	//Ambient
	vec3 ambient = vec3(_Properties.ambientK, _Properties.ambientK, _Properties.ambientK);
	FragColor = (texture(_Texture,fs_in.UV) * vec4((ambient+specular+diffuse),0));
}