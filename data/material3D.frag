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

varying vec3 v_ecNormal;

void main(void) {
	// Normalize v_ecNormal
	vec3 ecNormal = v_ecNormal / length(v_ecNormal);
	
	float ecNormalDotLightDirection = max(0.0, dot(ecNormal, EW_directionalLight.direction));
	float ecNormalDotLightHalfplane = max(0.0, dot(ecNormal, EW_directionalLight.halfplane));
	
	// Calculate ambient light
	vec4 ambientLight = EW_directionalLight.ambientColor * EW_material.ambientFactor;
	
	// Calculate diffuse light
	vec4 diffuseLight = ecNormalDotLightDirection * EW_directionalLight.diffuseColor * EW_material.diffuseFactor;
	
	// Calculate specular light
	vec4 specularLight = EW_directionalLight.specularColor * EW_material.specularFactor;
	
	if (ecNormalDotLightHalfplane > 0.0) {
		specularLight = pow(ecNormalDotLightHalfplane, EW_material.shininess) * EW_directionalLight.specularColor * EW_material.specularFactor;
		specularLight = EW_directionalLight.specularColor * EW_material.specularFactor;
	}
	vec4 light = ambientLight + diffuseLight + specularLight;
	gl_FragColor = light;
}
