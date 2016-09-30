/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

namespace ege {
	/**
	 * @not_in_doc
	 */
	class Face {
		public:
			int8_t m_nbElement;
			int32_t m_vertex[3];
			int32_t m_uv[3];
			int32_t m_normal[3];
			int32_t m_color[3];
		public:
			Face() :
			  m_nbElement(1) {
				m_vertex[0] = -1;
				m_vertex[1] = -1;
				m_vertex[2] = -1;
				m_uv[0] = -1;
				m_uv[1] = -1;
				m_uv[2] = -1;
				m_normal[0] = -1;
				m_normal[1] = -1;
				m_normal[2] = -1;
				m_color[0] = -1;
				m_color[1] = -1;
				m_color[2] = -1;
			};
			Face(int32_t _v1, int32_t _t1,
			     int32_t _v2, int32_t _t2,
			     int32_t _v3, int32_t _t3) :
			  m_nbElement(3) {
				m_vertex[0] = _v1;
				m_vertex[1] = _v2;
				m_vertex[2] = _v3;
				m_uv[0] = _t1;
				m_uv[1] = _t2;
				m_uv[2] = _t3;
				m_normal[0] = -1;
				m_normal[1] = -1;
				m_normal[2] = -1;
				m_color[0] = -1;
				m_color[1] = -1;
				m_color[2] = -1;
			};
			Face(int32_t _v1, int32_t _t1, int32_t _n1,
			     int32_t _v2, int32_t _t2, int32_t _n2,
			     int32_t _v3, int32_t _t3, int32_t _n3) :
			  m_nbElement(3) {
				m_vertex[0] = _v1;
				m_vertex[1] = _v2;
				m_vertex[2] = _v3;
				m_uv[0] = _t1;
				m_uv[1] = _t2;
				m_uv[2] = _t3;
				m_normal[0] = _n1;
				m_normal[1] = _n2;
				m_normal[2] = _n3;
				m_color[0] = -1;
				m_color[1] = -1;
				m_color[2] = -1;
			};
			void setVertex(int32_t _v1) {
				m_nbElement = 1;
				m_vertex[0] = _v1;
			}
			void setVertex(int32_t _v1, int32_t _v2) {
				m_nbElement = 2;
				m_vertex[0] = _v1;
				m_vertex[1] = _v2;
			}
			void setVertex(int32_t _v1, int32_t _v2, int32_t _v3) {
				m_nbElement = 3;
				m_vertex[0] = _v1;
				m_vertex[1] = _v2;
				m_vertex[2] = _v3;
			}
			void setTexture(int32_t _t1, int32_t _t2, int32_t _t3) {
				m_uv[0] = _t1;
				m_uv[1] = _t2;
				m_uv[2] = _t3;
			}
			void setNormal(int32_t _n1, int32_t _n2, int32_t _n3) {
				m_normal[0] = _n1;
				m_normal[1] = _n2;
				m_normal[2] = _n3;
			}
			void setColor(int32_t _c1, int32_t _c2, int32_t _c3) {
				m_color[0] = _c1;
				m_color[1] = _c2;
				m_color[2] = _c3;
			}
	};
}

