/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <gale/resource/Manager.hpp>
#include <ege/Material.hpp>
#include <ege/debug.hpp>

ege::MaterialGlId::MaterialGlId() :
  m_GL_ambientFactor(0),
  m_GL_diffuseFactor(0),
  m_GL_specularFactor(0),
  m_GL_shininess(0),
  m_GL_texture0(0) {
	// nothing to do else ...
}

void ege::MaterialGlId::link(ememory::SharedPtr<gale::resource::Program> _prog, const etk::String& _baseName) {
	if (_prog == null) {
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
  m_renderMode(gale::openGL::renderMode::triangle),
  m_texture0(null) {
	// nothing to do else ...
}

ege::Material::~Material() {
	
}

void ege::Material::draw(ememory::SharedPtr<gale::resource::Program> _prog, const MaterialGlId& _glID) {
	EGE_VERBOSE("draw Material : (start)");
	_prog->uniform4(_glID.m_GL_ambientFactor, m_ambientFactor);
	_prog->uniform4(_glID.m_GL_diffuseFactor, m_diffuseFactor);
	_prog->uniform4(_glID.m_GL_specularFactor, m_specularFactor);
	_prog->uniform1f(_glID.m_GL_shininess, m_shininess);
	if (m_texture0 != null) {
		EGE_VERBOSE("    set texture: " << _glID.m_GL_texture0 << " " << m_texture0->getId());
		_prog->setTexture0(_glID.m_GL_texture0, m_texture0->getRendererId());
		#if DEBUG
			if (_prog->checkIdValidity(_glID.m_GL_texture0) == false) {
				EGE_ERROR("try to set texture on a unexistant shader interface (wrong ID)");
			}
		#endif
	} else {
		#if DEBUG
			if (_prog->checkIdValidity(_glID.m_GL_texture0) == true) {
				EGE_ERROR("Missing texture to send on the shader ...");
			}
		#endif
	}
	EGE_VERBOSE("draw Material: ( end )");
}

void ege::Material::setAmbientFactor(const vec4& _val) {
	m_ambientFactor = _val;
}

void ege::Material::setDiffuseFactor(const vec4& _val) {
	//EGE_ERROR("****************  set difuse factor:" << _val);
	m_diffuseFactor = _val;
}

void ege::Material::setSpecularFactor(const vec4& _val) {
	m_specularFactor = _val;
}

void ege::Material::setShininess(float _val) {
	m_shininess = _val;
}

void ege::Material::setTexture0(const etk::Uri& _uri) {
	ivec2 tmpSize(256, 256);
	if (_uri.isEmpty() == false) {
		// prevent overloard error :
		ememory::SharedPtr<ewol::resource::Texture> tmpCopy = m_texture0;
		m_texture0 = ewol::resource::TextureFile::create(_uri, tmpSize);
		if (m_texture0 == null) {
			EGE_ERROR("Can not load specific texture : " << _uri);
			// retreave previous texture:
			m_texture0 = tmpCopy;
			if (m_texture0 != null) {
				return;
			}
		}
	} else {
		m_texture0.reset();
	}
}

void ege::Material::setTexture0Magic(const ivec2& _size) {
	// create a simple custum texture :
	m_texture0 = ewol::resource::Texture::create();
	if (m_texture0 != null) {
		setImageSize(_size);
		egami::Image& img = m_texture0->get();
		for (int32_t xxx=0; xxx<_size.x(); ++xxx) {
			for (int32_t yyy=0; yyy<_size.y(); ++yyy) {
				img.set(ivec2(xxx,yyy), etk::Color<>(1,0,0,1));
			}
		}
	} else {
		EGE_ERROR("Can not create empty stupid texture ...");
	}
}

enum gale::openGL::renderMode ege::Material::getRenderModeOpenGl() {
	return m_renderMode;
}
enum gale::openGL::renderMode ege::Material::getRenderMode() {
	return m_renderMode;
}

void ege::Material::setRenderMode(enum gale::openGL::renderMode _val) {
	switch (_val) {
		case gale::openGL::renderMode::point:
			break;
		case gale::openGL::renderMode::line:
			break;
		case gale::openGL::renderMode::lineStrip:
			EGE_INFO("Does not support " << _val << " auto convert it in 'LINE'");
			_val = gale::openGL::renderMode::line;
			break;
		case gale::openGL::renderMode::lineLoop:
			EGE_INFO("Does not support " << _val << " auto convert it in 'LINE'");
			_val = gale::openGL::renderMode::line;
			break;
		case gale::openGL::renderMode::triangle:
			break;
		case gale::openGL::renderMode::triangleStrip:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = gale::openGL::renderMode::triangle;
			break;
		case gale::openGL::renderMode::triangleFan:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = gale::openGL::renderMode::triangle;
			break;
		case gale::openGL::renderMode::quad:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = gale::openGL::renderMode::triangle;
			break;
		case gale::openGL::renderMode::quadStrip:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = gale::openGL::renderMode::triangle;
			break;
		case gale::openGL::renderMode::polygon:
			EGE_ERROR("Does not support " << _val << " try convert it in 'TRIANGLE'");
			_val = gale::openGL::renderMode::triangle;
			break;
	}
	m_renderMode = _val;
}

void ege::Material::setImageSize(const ivec2& _newSize) {
	if (m_texture0 == null){
		return;
	}
	m_texture0->setImageSize(_newSize);
}

egami::Image* ege::Material::get() {
	if (m_texture0 == null){
		return null;
	}
	return &m_texture0->get();
}

void ege::Material::flush() {
	if (m_texture0 == null){
		return;
	}
	m_texture0->flush();
}

