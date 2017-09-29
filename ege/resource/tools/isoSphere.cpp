/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/tools/isoSphere.hpp>

void ege::isoSphere::create(etk::Map<etk::String,ememory::SharedPtr<ege::Material>>& _materials, etk::Map<etk::String,FaceIndexing>& _listFaces, etk::Vector<vec3>& _listVertex, etk::Vector<vec2>& _listUV,
                            const etk::String& _materialName, int32_t _recursionLevel) {
	_recursionLevel = etk::max(_recursionLevel, 3);
	float size = 1.0f;
	//Add top and buttom point:
	_listVertex.pushBack(vec3(0.0f, 0.0f, size)); // 0 (top)
	_listVertex.pushBack(vec3(0.0f, 0.0f, -size)); // 1 (buttom)
	_listUV.pushBack(vec2(0.5, 1.0 )); // 0
	_listUV.pushBack(vec2(0.5, 0.0 )); // 1
	
	for (int32_t iii=0; iii<_recursionLevel; iii++) {
		
		
	}

}
