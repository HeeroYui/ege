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

void ege::MaterialGlId::link(const std::shared_ptr<ewol::resource::Program>& _prog, const std::string& _baseName) {
	if (nullptr == _prog) {
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
  m_renderMode(ewol::openGL::renderTriangle),
  m_texture0(nullptr) {
	// nothing to do else ...
}

ege::Material::~Material() {
	
}

void ege::Material::draw(const std::shared_ptr<ewol::resource::Program>& _prog, const MaterialGlId& _glID) {
	_prog->uniform4(_glID.m_GL_ambientFactor, m_ambientFactor);
	_prog->uniform4(_glID.m_GL_diffuseFactor, m_diffuseFactor);
	_prog->uniform4(_glID.m_GL_specularFactor, m_specularFactor);
	_prog->uniform1f(_glID.m_GL_shininess, m_shininess);
	if (nullptr != m_texture0) {
		_prog->setTexture0(_glID.m_GL_texture0, m_texture0->getId());
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
}

void ege::Material::setTexture0(const std::string& _filename) {
	ivec2 tmpSize(256, 256);
	if (_filename != "") {
		// prevent overloard error :
		std::shared_ptr<ewol::resource::Texture> tmpCopy = m_texture0;
		m_texture0 = ewol::resource::TextureFile::create(_filename, tmpSize);
		if (m_texture0 == nullptr) {
			EGE_ERROR("Can not load specific texture : " << _filename);
			// retreave previous texture:
			m_texture0 = tmpCopy;
			if (m_texture0 != nullptr) {
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
	if (m_texture0 != nullptr) {
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

int32_t ege::Material::getRenderModeOpenGl() {
	return static_cast<int32_t>(m_renderMode);
}

void ege::Material::setRenderMode(enum ewol::openGL::renderMode _val) {
	switch (_val) {
		case ewol::openGL::renderPoint:
			break;
		case ewol::openGL::renderLine:
			break;
		case ewol::openGL::renderLineStrip:
			EGE_INFO("Does not support " << _val << " auto convert it in 'LINE'");
			_val = ewol::openGL::renderLine;
			break;
		case ewol::openGL::renderLineLoop:
			EGE_INFO("Does not support " << _val << " auto convert it in 'LINE'");
			_val = ewol::openGL::renderLine;
			break;
		case ewol::openGL::renderTriangle:
			break;
		case ewol::openGL::renderTriangleStrip:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = ewol::openGL::renderTriangle;
			break;
		case ewol::openGL::renderTriangleFan:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = ewol::openGL::renderTriangle;
			break;
		case ewol::openGL::renderQuad:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = ewol::openGL::renderTriangle;
			break;
		case ewol::openGL::renderQuadStrip:
			EGE_INFO("Does not support " << _val << " auto convert it in 'TRIANGLE'");
			_val = ewol::openGL::renderTriangle;
			break;
		case ewol::openGL::renderPolygon:
			EGE_ERROR("Does not support " << _val << " try convert it in 'TRIANGLE'");
			_val = ewol::openGL::renderTriangle;
			break;
	}
	m_renderMode = _val;
}

