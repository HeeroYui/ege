/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ege/Light.hpp>
#include <ege/debug.hpp>

ege::Light::Light() :
  m_direction(0,0,0),
  m_halfplane(0,0,0),
  m_ambientColor(0,0,0,0),
  m_diffuseColor(0,0,0,0),
  m_specularColor(0,0,0,0),
  m_GL_direction(0),
  m_GL_halfplane(0),
  m_GL_ambientColor(0),
  m_GL_diffuseColor(0),
  m_GL_specularColor(0) {
	// nothing to do else ...
}

ege::Light::~Light() {
	
}

void ege::Light::link(ememory::SharedPtr<gale::resource::Program> _prog, const std::string& _baseName) {
	if (_prog == nullptr) {
		return;
	}
	m_GL_direction = _prog->getUniform(_baseName+".direction");
	m_GL_halfplane = _prog->getUniform(_baseName+".halfplane");
	m_GL_ambientColor = _prog->getUniform(_baseName+".ambientColor");
	m_GL_diffuseColor = _prog->getUniform(_baseName+".diffuseColor");
	m_GL_specularColor = _prog->getUniform(_baseName+".specularColor");
}

void ege::Light::draw(ememory::SharedPtr<gale::resource::Program> _prog) {
	_prog->uniform3(m_GL_direction, m_direction);
	_prog->uniform3(m_GL_halfplane, m_halfplane);
	_prog->uniform4(m_GL_ambientColor, m_ambientColor);
	_prog->uniform4(m_GL_diffuseColor, m_diffuseColor);
	_prog->uniform4(m_GL_specularColor, m_specularColor);
}

std::ostream& ege::operator <<(std::ostream& _os, const ege::Light& _obj) {
	_os << "light:{";
	_os << "dir=" << _obj.m_direction;
	_os << " halfplan=" << _obj.m_halfplane;
	_os << " color:{ anbiant=" << _obj.m_ambientColor;
	_os << " diffuse=" << _obj.m_diffuseColor;
	_os << " specular=" << _obj.m_specularColor;
	_os << "}}";
	return _os;
}

