/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/Mesh.hpp>

ememory::SharedPtr<ege::resource::Mesh> ege::resource::Mesh::createCube(float _size, const etk::String& _materialName, const etk::Color<float>& _color) {
	return createCube(vec3(_size, _size, _size), _materialName, _color);
}

ememory::SharedPtr<ege::resource::Mesh> ege::resource::Mesh::createCube(const vec3& _size, const etk::String& _materialName, const etk::Color<float>& _color) {
	EGE_VERBOSE(" create a cube _size=" << _size << " _materialName=" << _materialName << " _color=" << _color);
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("---", "DATA:///color3.prog");
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
		
		out->addQuad(_materialName, vec3(-1,-1, 1)*_size, vec3(-1,-1,-1)*_size, vec3( 1,-1,-1)*_size, vec3( 1,-1, 1)*_size, _color);
		out->addQuad(_materialName, vec3(-1, 1,-1)*_size, vec3(-1, 1, 1)*_size, vec3( 1, 1, 1)*_size, vec3( 1, 1,-1)*_size, _color);
		out->addQuad(_materialName, vec3(-1, 1,-1)*_size, vec3(-1,-1,-1)*_size, vec3(-1,-1, 1)*_size, vec3(-1, 1, 1)*_size, _color);
		out->addQuad(_materialName, vec3( 1,-1,-1)*_size, vec3( 1, 1,-1)*_size, vec3( 1, 1, 1)*_size, vec3( 1,-1, 1)*_size, _color);
		out->addQuad(_materialName, vec3(-1,-1,-1)*_size, vec3(-1, 1,-1)*_size, vec3( 1, 1,-1)*_size, vec3( 1,-1,-1)*_size, _color);
		out->addQuad(_materialName, vec3(-1, 1, 1)*_size, vec3(-1,-1, 1)*_size, vec3( 1,-1, 1)*_size, vec3( 1, 1, 1)*_size, _color);
		out->setNormalMode(ege::resource::Mesh::normalMode::face);
		out->calculateNormaleFace(_materialName);
		// generate the VBO
		out->generateVBO();
	} else {
		EGE_ERROR("can not create the basic mesh interface");
	}
	return out;
}