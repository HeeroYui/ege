/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Vector4D.hpp>
#include <gale/resource/Program.hpp>
#include <ewol/resource/Image.hpp>

namespace ege {
	/**
	 * @not_in_doc
	 */
	class MaterialGlId {
		public:
			// GL index
			int32_t m_GL_ambientFactor;
			int32_t m_GL_diffuseFactor;
			int32_t m_GL_specularFactor;
			int32_t m_GL_shininess;
			int32_t m_GL_texture0;
			MaterialGlId();
			void link(ememory::SharedPtr<gale::resource::Program> _prog, const std::string& _baseName);
	};
	
	
	class Material {
		private:
			// values
			vec4 m_ambientFactor;
			vec4 m_diffuseFactor;
			vec4 m_specularFactor;
			float m_shininess;
			enum gale::openGL::renderMode m_renderMode; // Select Render mode (triangle/Line/point ...)
			ememory::SharedPtr<ewol::resource::Texture> m_texture0;
		public:
			std::vector<uint32_t> m_listIndexFaces;
		public:
			Material();
			~Material();
			void draw(ememory::SharedPtr<gale::resource::Program> _prog, const ege::MaterialGlId& _glID);
			void setAmbientFactor(const vec4& _val) {
				m_ambientFactor = _val;
			}
			void setDiffuseFactor(const vec4& _val) {
				m_diffuseFactor = _val;
			}
			void setSpecularFactor(const vec4& _val) {
				m_specularFactor = _val;
			}
			void setShininess(float _val) {
				m_shininess = _val;
			}
			void setRenderMode(enum gale::openGL::renderMode _val);
			enum gale::openGL::renderMode getRenderModeOpenGl();
			enum gale::openGL::renderMode getRenderMode() {
				return m_renderMode;
			}
			void setTexture0(const std::string& _filename);
			void setTexture0Magic(const ivec2& _size);
			
			void setImageSize(const ivec2& _newSize) {
				if (m_texture0 == nullptr){
					return;
				}
				m_texture0->setImageSize(_newSize);
			};
			// get the reference on this image to draw nomething on it ...
			egami::Image* get() {
				if (m_texture0 == nullptr){
					return nullptr;
				}
				return &m_texture0->get();
			};
			// flush the data to send it at the openGl system
			void flush() {
				if (m_texture0 == nullptr){
					return;
				}
				m_texture0->flush();
			};
	};
}


