/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/Mesh.hpp>

ememory::SharedPtr<ege::resource::Mesh> ege::resource::Mesh::createCone(float _radius, float _size, const etk::String& _materialName, const etk::Color<float>& _color, int32_t _lats, int32_t _longs) {
	EGE_VERBOSE(" create a cylinder _size=" << _size << " _materialName=" << _materialName << " _color=" << _color);
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("---", "DATA:color3.prog");
	if (out != null) {
		ememory::SharedPtr<ege::Material> material = ememory::makeShared<ege::Material>();
		// set the element material properties :
		material->setAmbientFactor(vec4(1,1,1,1));
		material->setDiffuseFactor(vec4(0,0,0,1));
		material->setSpecularFactor(vec4(0,0,0,1));
		material->setShininess(1);
		material->setRenderMode(gale::openGL::renderMode::triangle);
		out->addMaterial(_materialName, material);
		
		out->addFaceIndexing(_materialName);
		
		// center to border (TOP)
		for(int32_t jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			vec3 v1 = vec3(0.0f, 0.0f, _size/2);
			
			float x = cos(lng)*_radius;
			float y = sin(lng)*_radius;
			vec3 v2 = vec3(x, y, -_size/2);
			
			lng = 2.0f * M_PI * float(jjj) / _longs;
			x = cos(lng)*_radius;
			y = sin(lng)*_radius;
			vec3 v3 = vec3(x, y, -_size/2);
			out->addTriangle(_materialName, v1, v2, v3, _color);
		}
		// center to border (BUTTOM)
		for(int32_t jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			
			vec3 v1 = vec3(0.0f, 0.0f, -_size/2);
			
			float x = cos(lng)*_radius;
			float y = sin(lng)*_radius;
			vec3 v2 = vec3(x, y, -_size/2);
			
			lng = 2.0f * M_PI * float(jjj) / _longs;
			x = cos(lng)*_radius;
			y = sin(lng)*_radius;
			vec3 v3 = vec3(x, y, -_size/2);
			out->addTriangle(_materialName, v1, v3, v2, _color);
		}
		out->setNormalMode(ege::resource::Mesh::normalMode::face);
		out->calculateNormaleFace(_materialName);
		// generate the VBO
		out->generateVBO();
	} else {
		EGE_ERROR("can not create the basic mesh interface");
	}
	return out;
}