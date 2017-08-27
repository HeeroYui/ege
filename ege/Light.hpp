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

namespace ege {
	class Light {
		private:
			// values
			vec3 m_direction;
			vec3 m_halfplane;
			vec4 m_ambientColor;
			vec4 m_diffuseColor;
			vec4 m_specularColor;
		private:
			// GL index
			int32_t m_GL_direction;
			int32_t m_GL_halfplane;
			int32_t m_GL_ambientColor;
			int32_t m_GL_diffuseColor;
			int32_t m_GL_specularColor;
		public:
			Light();
			~Light();
			void link(ememory::SharedPtr<gale::resource::Program> _prog, const etk::String& _baseName);
			void draw(ememory::SharedPtr<gale::resource::Program> _prog);
			void setDirection(const vec3& val) {
				m_direction = val;
			}
			void setHalfPlane(const vec3& val) {
				m_halfplane = val;
			}
			void setAmbientColor(const vec4& val) {
				m_ambientColor = val;
			}
			void setDiffuseColor(const vec4& val) {
				m_diffuseColor = val;
			}
			void setSpecularColor(const vec4& val) {
				m_specularColor = val;
			}
			
		friend etk::Stream& operator <<(etk::Stream& _os, const ege::Light& _obj);
	};
	etk::Stream& operator <<(etk::Stream& _os, const ege::Light& _obj);
}

