/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_TOOLS_VIEWBOX_H__
#define __EGE_TOOLS_VIEWBOX_H__

#include <ege/resource/tools/Face.h>
#include <ege/resource/tools/FaceIndexing.h>
#include <etk/Hash.h>
#include <ege/Material.h>

namespace ege {
	namespace viewBox {
		void create(etk::Hash<ege::Material*>& _materials, etk::Hash<FaceIndexing>& _listFaces, std::vector<vec3>& _listVertex, std::vector<vec2>& _listUV,
		            const std::string& _materialName, float _size);
	};
};


#endif

