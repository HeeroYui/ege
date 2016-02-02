/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#pragma once

namespace ege {
	class FaceIndexing {
		public:
			std::vector<Face> m_faces;
			std::vector<uint32_t> m_index;
	};
}

