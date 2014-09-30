/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_FACE_INDEXING_H__
#define __EGE_FACE_INDEXING_H__

namespace ege {
	class FaceIndexing {
		public:
			std::vector<Face> m_faces;
			std::vector<uint32_t> m_index;
	};
};

#endif

