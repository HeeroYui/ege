/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_FACE_H__
#define __EGE_FACE_H__

namespace ege {
	/**
	 * @not-in-doc
	 */
	class Face {
		public:
			int32_t m_vertex[3];
			int32_t m_uv[3];
			int32_t m_normal[3];
		public:
			Face() {};
			Face(int32_t v1, int32_t t1,
			     int32_t v2, int32_t t2,
			     int32_t v3, int32_t t3) {
				m_vertex[0] = v1;
				m_vertex[1] = v2;
				m_vertex[2] = v3;
				m_uv[0] = t1;
				m_uv[1] = t2;
				m_uv[2] = t3;
				m_normal[0] = -1;
				m_normal[1] = -1;
				m_normal[2] = -1;
			};
			Face(int32_t v1, int32_t t1, int32_t n1,
			     int32_t v2, int32_t t2, int32_t n2,
			     int32_t v3, int32_t t3, int32_t n3) {
				m_vertex[0] = v1;
				m_vertex[1] = v2;
				m_vertex[2] = v3;
				m_uv[0] = t1;
				m_uv[1] = t2;
				m_uv[2] = t3;
				m_normal[0] = n1;
				m_normal[1] = n2;
				m_normal[2] = n3;
			};
	};
};

#endif
