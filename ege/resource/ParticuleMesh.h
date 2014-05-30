/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_RESOURCE_PARTICULE_MESH_H__
#define __EGE_RESOURCE_PARTICULE_MESH_H__

#include <ege/resource/Mesh.h>

namespace ege {
	namespace resource {
		class ParticuleMesh : public ege::resource::Mesh {
			protected:
				int32_t m_GLMainColor;
			public:
				ParticuleMesh(const std::string& _fileName, const std::string& _shaderName);
				virtual ~ParticuleMesh();
			public:
				virtual void draw(mat4& _positionMatrix,
				                  const etk::Color<float>& _mainColor,
				                  bool _enableDepthTest = true,
				                  bool _enableDepthUpdate = true);
			public:
				/**
				 * @brief keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the ewol mesh file.
				 * @return pointer on the resource or nullptr if an error occured.
				 */
				static ewol::object::Shared<ege::resource::ParticuleMesh> keep(const std::string& _meshName, const std::string& _shaderName="DATA:ParticuleMesh.prog");
		};
	};
};


#endif
