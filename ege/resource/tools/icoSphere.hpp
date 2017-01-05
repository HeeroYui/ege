/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/resource/tools/Face.hpp>
#include <ege/resource/tools/FaceIndexing.hpp>
#include <etk/Hash.hpp>
#include <ege/Material.hpp>

namespace ege {
	namespace icoSphere {
		void create(etk::Hash<ememory::SharedPtr<ege::Material>>& _materials, etk::Hash<FaceIndexing>& _listFaces, std::vector<vec3>& _listVertex, std::vector<vec2>& _listUV,
		            const std::string& _materialName, float _size, int32_t _recursionLevel);
	}
}

