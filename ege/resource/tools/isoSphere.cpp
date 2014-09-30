/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/resource/tools/isoSphere.h>

void ege::isoSphere::create(etk::Hash<ege::Material*>& _materials, etk::Hash<FaceIndexing>& _listFaces, std::vector<vec3>& _listVertex, std::vector<vec2>& _listUV,
                            const std::string& _materialName, int32_t _recursionLevel) {
	_recursionLevel = std::max(_recursionLevel, 3);
	float size = 1.0f;
	//Add top and buttom point:
	_listVertex.push_back(vec3(0.0f, 0.0f, size)); // 0 (top)
	_listVertex.push_back(vec3(0.0f, 0.0f, -size)); // 1 (buttom)
	_listUV.push_back(vec2(0.5, 1.0 )); // 0
	_listUV.push_back(vec2(0.5, 0.0 )); // 1
	
	for (int32_t iii=0; iii<_recursionLevel; iii++) {
		
		
	}

}
