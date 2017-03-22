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
    vec4 ambientFactor; // color of the material with a reflection factor (0.2 for example) ==> light the back of the object
    vec4 diffuseFactor; // Direct exposition of the object by the light
    vec4 specularFactor; // Shining the Object with the light
    float shininess;
};

// Light
uniform DirectionalLight EW_directionalLight;
// Material
uniform Material EW_material;

varying vec3 v_ecNormal;

void main(void) {
	// Normalize v_ecNormal
	vec3 ecNormal = normalize(v_ecNormal);
	
	// Calculate ambient light
	vec4 ambientLight = EW_directionalLight.ambientColor * EW_material.ambientFactor;
	
	float ecNormalDotLightDirection = max(0.0, dot(ecNormal, EW_directionalLight.direction));
	//float ecNormalDotLightDirection = (dot(ecNormal, EW_directionalLight.direction)+1.0) * 0.5;
	
	// Calculate diffuse light
	vec4 diffuseLight = ecNormalDotLightDirection * EW_directionalLight.diffuseColor * EW_material.diffuseFactor;
	//vec4 diffuseLight = vec4(0.0, 0.0, 0.0, 0.0);
	
	// Calculate specular light
	vec4 specularLight = EW_directionalLight.specularColor * EW_material.specularFactor;
	
	float ecNormalDotLightHalfplane = dot(ecNormal, EW_directionalLight.halfplane);
	if (ecNormalDotLightHalfplane > 0.0) {
		specularLight = pow(ecNormalDotLightHalfplane, EW_material.shininess) * EW_directionalLight.specularColor * EW_material.specularFactor;
		specularLight = EW_directionalLight.specularColor * EW_material.specularFactor;
	}
	
	vec4 light = ambientLight + diffuseLight + specularLight;
	gl_FragColor = light;
}
