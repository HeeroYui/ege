#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif
struct DirectionalLight {
    vec3 direction;
    vec3 halfplane;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
};

struct Material {
    vec4 ambientFactor;
    vec4 diffuseFactor;
    vec4 specularFactor;
    float shininess;
};

// Light
uniform DirectionalLight EW_directionalLight;
// Material
uniform Material EW_material;

// Input :
uniform sampler2D EW_texID;
uniform vec4 EW_mainColor;

varying vec2 f_texcoord;
varying vec3 v_ecNormal;

void main(void) {
	vec4 tmpElementColor = texture2D(EW_texID, f_texcoord)*EW_mainColor;
	
	// Calculate ambient light
	vec4 ambientLight = EW_directionalLight.ambientColor * EW_material.ambientFactor;
	
	vec4 light = ambientLight;
	
	gl_FragColor = tmpElementColor * light;
}
