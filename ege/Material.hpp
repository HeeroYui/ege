/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Vector4D.hpp>
#include <gale/resource/Program.hpp>
#include <ewol/resource/TextureFile.hpp>
#include <ege/debug.hpp>

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
			void link(ememory::SharedPtr<gale::resource::Program> _prog, const etk::String& _baseName);
	};
	class Material {
		private:
			// values
			vec4 m_ambientFactor;
			vec4 m_diffuseFactor;
			vec4 m_specularFactor;
			float m_shininess;
			enum gale::openGL::renderMode m_renderMode; //!< Select Render mode (triangle/Line/point ...)
			ememory::SharedPtr<ewol::resource::Texture> m_texture0;
		public:
			etk::Vector<uint32_t> m_listIndexFaces;
		public:
			Material();
			~Material();
			void draw(ememory::SharedPtr<gale::resource::Program> _prog, const ege::MaterialGlId& _glID);
			void setAmbientFactor(const vec4& _val);
			void setDiffuseFactor(const vec4& _val);
			void setSpecularFactor(const vec4& _val);
			void setShininess(float _val);
			void setRenderMode(enum gale::openGL::renderMode _val);
			enum gale::openGL::renderMode getRenderModeOpenGl();
			enum gale::openGL::renderMode getRenderMode();
			void setTexture0(const etk::Uri& _uri);
			void setTexture0Magic(const ivec2& _size);
			void setImageSize(const ivec2& _newSize);
			// get the reference on this image to draw nomething on it ...
			egami::Image* get();
			// flush the data to send it at the openGl system
			void flush();
	};
}


