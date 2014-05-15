/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ewol/resource/Manager.h>
#include <ege/Material.h>
#include <ege/debug.h>

ege::MaterialGlId::MaterialGlId() :
  m_GL_ambientFactor(0),
  m_GL_diffuseFactor(0),
  m_GL_specularFactor(0),
  m_GL_shininess(0),
  m_GL_texture0(0) {
	// nothing to do else ...
}


void ege::MaterialGlId::link(ewol::resource::Program* _prog, const std::string& _baseName) {
	if (NULL == _prog) {
		return;
	}
	m_GL_ambientFactor = _prog->getUniform(_baseName+".ambientFactor");
	m_GL_diffuseFactor = _prog->getUniform(_baseName+".diffuseFactor");
	m_GL_specularFactor = _prog->getUniform(_baseName+".specularFactor");
	m_GL_shininess = _prog->getUniform(_baseName+".shininess");
	m_GL_texture0 = _prog->getUniform("EW_texID");
}

ege::Material::Material() :
  m_ambientFactor(1,1,1,1),
  m_diffuseFactor(0,0,0,1),
  m_specularFactor(0,0,0,1),
  m_shininess(1),
  m_texture0(NULL) {
	// nothing to do else ...
}
ege::Material::~Material() {
	if(NULL!=m_texture0) {
		ewol::resource::TextureFile::release(m_texture0);
	}
}

void ege::Material::draw(ewol::resource::Program* _prog, const MaterialGlId& _glID) {
	_prog->uniform4(_glID.m_GL_ambientFactor, m_ambientFactor);
	_prog->uniform4(_glID.m_GL_diffuseFactor, m_diffuseFactor);
	_prog->uniform4(_glID.m_GL_specularFactor, m_specularFactor);
	_prog->uniform1f(_glID.m_GL_shininess, m_shininess);
	if (NULL != m_texture0) {
		_prog->setTexture0(_glID.m_GL_texture0, m_texture0->getId());
	}
}

void ege::Material::setTexture0(const std::string& _filename) {
	ivec2 tmpSize(256, 256);
	// prevent overloard error :
	ewol::resource::TextureFile* tmpCopy = m_texture0;
	m_texture0 = ewol::resource::TextureFile::keep(_filename, tmpSize);
	if (NULL == m_texture0 ) {
		EGE_ERROR("Can not load specific texture : " << _filename);
		// retreave previous texture:
		m_texture0 = tmpCopy;
		return;
	}
	if (NULL != tmpCopy) {
		// really release previous texture. In case of same texture loading, then we did not have reload it .. just increase and decrease index...
		ewol::resource::TextureFile::release(tmpCopy);
	}
}



