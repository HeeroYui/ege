/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

namespace ege {
	class FaceIndexing {
		public:
			std::vector<Face> m_faces;
			std::vector<uint32_t> m_index; //!< index of the vertex in the VBO not in the Mesh.m_listVertex
	};
}

